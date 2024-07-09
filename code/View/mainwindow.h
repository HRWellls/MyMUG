#ifndef MYMUG_MAINWINDOW_H
#define MYMUG_MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QVector>
#include "track.h"
#include "noteBlock.h"
#include "KeyEvent.h"
#include "judgementLine.h"
#include "ComboBlock.h"
#include "ScoreBlock.h"
#include "scoreTitleBlock.h"
#include "backgroundBlock.h"
#include "../Common/Key.hpp"
#include "../Common/NoteInfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    bool* outputKey(); // 返回bool指针
    void setActiveNotes(std::vector<NoteInfo> *activeNotesPtr) {activeNotes = activeNotesPtr;} // 设置activeNotes
    void recieveActiveNotes(std::vector<NoteInfo> *activeNotesPtr);
    std::vector<NoteInfo>* getActiveNotes() const { return activeNotes; }

public slots:
    void updateNotes();
    void updateScore(int score);
    void updateCombo(int combo);
    void createBackground(const QString &pngPath);
    void createScoreTitleBlock(const QString &title);
    void createTracks();
    void createJudgementLine();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void keyStateChanged(const Key &key);
    void key0Pressed();
    void key1Pressed();
    void key2Pressed();
    void key3Pressed();
    void key0Released();
    void key1Released();
    void key2Released();
    void key3Released();

private:
    Ui::MainWindow *ui;
    QMap<int, Key*> activeKeys;  // 存储正在按下的键
    QList<Track*> tracks;
    QList<noteBlock*> noteBlocks;
    std::vector<NoteInfo> *activeNotes;  // 用于存储activeNotesPtr
    ScoreBlock *scoreBlock = nullptr; // 分数块
    backgroundBlock *background; // 背景块
    ComboBlock *comboBlock = nullptr; // Combo块

    void createComboBlock(int combo);
    void createScoreBlock(int score);
    void createNotes();
};

#endif //MYMUG_MAINWINDOW_H
