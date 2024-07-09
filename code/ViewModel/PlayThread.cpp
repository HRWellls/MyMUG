//
// Created by Frankoxer on 24-7-6.
//

#include "PlayThread.h"
#include "ViewModel.hpp" // 在这里包含ViewModel的定义

PlayThread::PlayThread(ViewModel* viewModel) : viewModel(viewModel) {}

void PlayThread::run() {
    if (viewModel) {
        auto it = viewModel->notes.begin();
        std::string songPath;
        songPath+="../resources/music/" + viewModel->title + ".wav";
        LPCSTR songPathChar = songPath.c_str();
        PlaySound(TEXT(songPathChar), nullptr, SND_FILENAME | SND_ASYNC);
        viewModel->gameStartTime = std::chrono::steady_clock::now();

        for(auto it: viewModel->keys) {
            it.updateStartTime(viewModel->gameStartTime);
        }

        while (true) {
            auto beforeLoopTime = std::chrono::steady_clock::now();

            int isHit = 4;
            viewModel->updateNotes(it);
            isHit = viewModel->updateKeyStates();
            if(isHit != 4) {
                int track = isHit;
                auto currentTime = std::chrono::steady_clock::now();
                int currentTimeStamp = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1, 1000>>>(currentTime - viewModel->gameStartTime).count();

                for (auto & note : viewModel->notes) {
                    if (!note.isJudged && note.getTrack()==track+1 && abs(note.getTimestamp() - currentTimeStamp)<= PERFECT_TIME) {
                        viewModel->point+= 100;
                        note.isJudged = true;
                        viewModel->comb++;
                        emit viewModel->updateCombo(viewModel->comb);
                        emit viewModel->updateScore(viewModel->point);
                        break;
                    } else if (!note.isJudged && note.getTrack()==track+1 && abs(note.getTimestamp() - currentTimeStamp)<= GOOD_TIME) {
                        viewModel->point+= 75;
                        viewModel->comb++;
                        emit viewModel->updateCombo(viewModel->comb);
                        note.isJudged = true;
                        emit viewModel->updateScore(viewModel->point);
                        break;
                    }
                }

            }

            // Check if any note has passed the judgement line without being hit
            auto currentTime = std::chrono::steady_clock::now();
            int currentTimeStamp = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1, 1000>>>(currentTime - viewModel->gameStartTime).count();
            for (auto & note : viewModel->notes) {
                if (!note.isJudged &&  currentTimeStamp - note.getTimestamp() > GOOD_TIME) {
                    viewModel->comb = 0; // Reset the comb count when a note is missed
                    emit viewModel->updateCombo(viewModel->comb);
                    note.isJudged = true; // Mark the note as judged
                    break;
                }
            }

            // 检查是否到达乐曲结束时间，如果是，则退出循环
            currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - viewModel->gameStartTime).count();
            if (elapsedTime > viewModel->song.getEndTime()) {
                std::cout << "Song ended. Exiting game loop." << std::endl;
                break;
            }
            if(viewModel->activeNotesPtr == nullptr) {
                std::cout << "activeNotesPtr is nullptr" << std::endl;
            }
            auto afterLoopTime = std::chrono::steady_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(TIME_INTERVAL) - (afterLoopTime - beforeLoopTime));
            emit viewModel->updateNoteView();
        }
    }
}