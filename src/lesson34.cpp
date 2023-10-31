#include <iomanip>
#include <iostream>

#include "framework1.h"

Uint8* gRecordingBuffer = nullptr;
Uint32 gBufferSize = 0;
Uint32 gBufferBytePosition = 0;
Uint32 gBufferByteMaxPosition = 0;
Uint32 gRecordingDeviceCount = 0;

void AudioRecordingCallback_(void* userdata, Uint8* stream, int len) {
  memcpy(&gRecordingBuffer[gBufferBytePosition], stream, len);
  gBufferBytePosition += len;
}

void AudioPlaybackCallback_(void* userdata, Uint8* stream, int len) {
  memcpy(stream, &gRecordingBuffer[gBufferBytePosition], len);
  gBufferBytePosition += len;
}

int main(int argc, char** argv) {
  Init();

  UTexture gPromptTexture, gStatTexture;
  UTexture gDeviceTexture[kMaxRecordingDevice];
  const SDL_Color text_color{0x0, 0x0, 0x0, 0xFF};
  gPromptTexture.LoadFromText("Select your recording device", text_color);

  gRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);
  SDL_AudioSpec received_recording_spec, received_playback_spec;

  check_error_custom(gRecordingDeviceCount < 1, "No audio recording device.");
  if (gRecordingDeviceCount > kMaxRecordingDevice) {
    gRecordingDeviceCount = kMaxRecordingDevice;
  }
  for (Uint32 i = 0; i < gRecordingDeviceCount; i++) {
    std::string s =
        std::to_string(i) + ": " + SDL_GetAudioDeviceName(i, SDL_TRUE);
    gDeviceTexture[i].LoadFromText(s.c_str(), text_color);
  }

  RecordingState current_state = RecordingState::kSelectingDevice;
  SDL_AudioDeviceID recording_device_id = 0;
  SDL_AudioDeviceID playback_device_id = 0;

  SDL_Event e;
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      switch (current_state) {
        case RecordingState::kSelectingDevice:
          if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
              Uint32 selected_index = e.key.keysym.sym - SDLK_0;
              if (selected_index < gRecordingDeviceCount) {
                SDL_AudioSpec desired_audio_spec;
                SDL_zero(desired_audio_spec);
                // following is recommended arguments for most platforms
                desired_audio_spec.freq = 44100;
                desired_audio_spec.format = AUDIO_F32;
                desired_audio_spec.channels = 2;
                desired_audio_spec.samples = 4096;
                desired_audio_spec.callback = AudioRecordingCallback_;
                recording_device_id = SDL_OpenAudioDevice(
                    SDL_GetAudioDeviceName(selected_index, SDL_TRUE), SDL_TRUE,
                    &desired_audio_spec, &received_recording_spec, 0);
                check_error(recording_device_id == 0);
                desired_audio_spec.callback = AudioPlaybackCallback_;
                playback_device_id =
                    SDL_OpenAudioDevice(nullptr, SDL_FALSE, &desired_audio_spec,
                                        &received_playback_spec, 0);
                check_error(playback_device_id == 0);

                const int bytes_per_sample =
                    received_recording_spec.channels *
                    (SDL_AUDIO_BITSIZE(received_recording_spec.format) / 8);
                const int bytes_per_seconds =
                    received_recording_spec.freq * bytes_per_sample;
                gBufferSize = kRecordBufferTime * bytes_per_seconds;
                gBufferByteMaxPosition = kMaxRecordingTime * bytes_per_seconds;
                gRecordingBuffer = new Uint8[gBufferSize];
                memset(gRecordingBuffer, 0, gBufferSize);
                gPromptTexture.TryFree();
                gPromptTexture.LoadFromText("Press 1 to record for 5 seconds",
                                            text_color);
                current_state = RecordingState::kStopped;
              }
            }
          }
          break;
        case RecordingState::kStopped:
          if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_1) {
            gBufferBytePosition = 0;
            SDL_PauseAudioDevice(recording_device_id, SDL_FALSE);
            gPromptTexture.TryFree();
            gPromptTexture.LoadFromText("Start recording...", text_color);
            current_state = RecordingState::kRecording;
          }
          break;
        case RecordingState::kRecorded:
          if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_1) {
              gBufferBytePosition = 0;
              SDL_PauseAudioDevice(playback_device_id, SDL_FALSE);
              gPromptTexture.TryFree();
              gPromptTexture.LoadFromText("Playing...", text_color);
              current_state = RecordingState::kPlayback;
            } else if (e.key.keysym.sym == SDLK_2) {
              gBufferBytePosition = 0;
              SDL_PauseAudioDevice(recording_device_id, SDL_FALSE);
              gPromptTexture.TryFree();
              gPromptTexture.LoadFromText("Recording...", text_color);
              current_state = RecordingState::kRecording;
            }
          }
          break;
        default:
          break;
      }
    }

    if (current_state == RecordingState::kRecording) {
      SDL_LockAudioDevice(recording_device_id);
      if (gBufferBytePosition > gBufferByteMaxPosition) {
        SDL_PauseAudioDevice(recording_device_id, SDL_TRUE);
        gPromptTexture.TryFree();
        gPromptTexture.LoadFromText(
            "Press 1 to playback. Press 2 to record again.", text_color);
        current_state = RecordingState::kRecorded;
      }
      SDL_UnlockAudioDevice(recording_device_id);
    } else if (current_state == RecordingState::kPlayback) {
      SDL_LockAudioDevice(playback_device_id);
      if (gBufferBytePosition > gBufferByteMaxPosition) {
        SDL_PauseAudioDevice(playback_device_id, SDL_TRUE);
        gPromptTexture.TryFree();
        gPromptTexture.LoadFromText(
            "Press 1 to playback. Press 2 to record again.", text_color);
        current_state = RecordingState::kRecorded;
      }
      SDL_UnlockAudioDevice(playback_device_id);
    }

    // the rms need to be linearly corrected
    // different device has differently 0 point and different steep
    // map the specific line to standard line that with k = 1 and 0 point = 0
    if (current_state == RecordingState::kRecorded) {
      double rms = 0.0;
      for (float* i = reinterpret_cast<float*>(gRecordingBuffer);
           i < reinterpret_cast<float*>(gRecordingBuffer + gBufferSize); i++) {
        rms += (*i) * (*i);
      }
      rms = std::sqrt(rms);
      std::stringstream ss;
      ss << std::setprecision(6) << rms;
      gStatTexture.TryFree();
      gStatTexture.LoadFromText(ss.str().c_str(), text_color);
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    int y_offset = 0;
    gPromptTexture.Render((kScreenWidth - gPromptTexture.GetWidth()) / 2, 0);
    y_offset += gPromptTexture.GetHeight();
    if (current_state == RecordingState::kSelectingDevice) {
      for (Uint32 i = 0; i < gRecordingDeviceCount; i++) {
        gDeviceTexture[i].Render(
            (kScreenWidth - gDeviceTexture[i].GetWidth()) / 2, y_offset);
        y_offset += gDeviceTexture[i].GetHeight();
      }
    } else if (current_state == RecordingState::kRecorded) {
      gStatTexture.Render((kScreenWidth - gStatTexture.GetWidth()) / 2,
                          y_offset);
      y_offset += gStatTexture.GetHeight();
    }

    SDL_RenderPresent(gRenderer);
  }

  Close();
  return 0;
}
