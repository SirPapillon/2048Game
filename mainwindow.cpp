#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QKeyEvent>

#include <QMessageBox>

#include <QPropertyAnimation>

#include <QThread>

#include <QProcess>

void StartNewPoint();
void CreateLabelAnimation(int target_index);
void ResizeLabelAnimation(int target_index);
int TotalScore = 0;
void setScore(int score, int target_index);
void removeL(QLabel * l);
void AnimationC(int source_index, int target_index);
int numbers_in_Table[16] = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
};

void set_Config_onLabel_byIndex(QLabel * target_lbl, int index, int isCreate);
void setValue_on_Gridindex(int index, int value, int source_index);
int generate_random_index();

void goRight();
void goLeft();
void goDown();
void goTop();

MainWindow::MainWindow(QWidget * parent): QMainWindow(parent), ui(new Ui::MainWindow) {

  ui -> setupUi(this);
  ui -> frame -> setVisible(false);

  NewGame();
}

MainWindow::~MainWindow() {
  delete ui;
}

// convert array's index to label widget
QLabel * MainWindow::indexToQLabel(int index) {
  QLabel * lbl;
  switch (index) {
  case 0:
    lbl = ui -> B11;
    break;
  case 1:
    lbl = ui -> B12;
    break;
  case 2:
    lbl = ui -> B13;
    break;
  case 3:
    lbl = ui -> B14;
    break;
  case 4:
    lbl = ui -> B21;
    break;
  case 5:
    lbl = ui -> B22;
    break;
  case 6:
    lbl = ui -> B23;
    break;
  case 7:
    lbl = ui -> B24;
    break;
  case 8:
    lbl = ui -> B31;
    break;
  case 9:
    lbl = ui -> B32;
    break;
  case 10:
    lbl = ui -> B33;
    break;
  case 11:
    lbl = ui -> B34;
    break;
  case 12:
    lbl = ui -> B41;
    break;
  case 13:
    lbl = ui -> B42;
    break;
  case 14:
    lbl = ui -> B43;
    break;
  case 15:
    lbl = ui -> B44;
    break;
  }
  return lbl;
}

// animation for creating new grid item - new label
void MainWindow::CreateLabelAnimation(int target_index) {
  QLabel * animation_lbl = indexToQLabel(target_index); // target_index to label widget
  QPropertyAnimation * animation = new QPropertyAnimation(animation_lbl, "size");
  animation -> setDuration(100); // duration of animation
  animation -> setStartValue(QSize(5, 5));
  animation -> setEndValue(QSize(98, 98));
  animation -> start();
}

// resize label when we put  Numbers together
void MainWindow::ResizeLabelAnimation(int target_index) {
  /***

      First step : resize 98 x 98 label to 110 x 110 label  in 100 ms
      Second step : resize 110 x 110 label to 98 x 98 label  in 100 ms

***/
  QLabel * animation_lbl = indexToQLabel(target_index);
  QPropertyAnimation * animation = new QPropertyAnimation(animation_lbl, "size");
  animation -> setDuration(100);
  animation -> setStartValue(QSize(98, 98));

  animation -> setEndValue(QSize(110, 110));
  animation -> start();
  connect(animation, & QPropertyAnimation::finished, this, [this, animation_lbl, target_index] {
    QLabel * animation_lbl = indexToQLabel(target_index);
    QPropertyAnimation * animation = new QPropertyAnimation(animation_lbl, "size");
    animation -> setDuration(100);
    animation -> setStartValue(QSize(110, 110));

    animation -> setEndValue(QSize(98, 98));
    animation -> start();
  });

}

// animation when labels transfer
void MainWindow::AnimationC(int source_index, int target_index) {

  /***
  First, we create a temp label on source_index like main label ,
  then we move that to target_index and destroy that
  ***/
  QLabel * startL = indexToQLabel(source_index);
  QLabel * endL = indexToQLabel(target_index);
  QLabel * tempLabel = new QLabel();
  tempLabel -> setGeometry(endL -> geometry());
  this -> layout() -> addWidget(tempLabel); // add tempLabel to layout
  tempLabel -> setStyleSheet("background-color: rgba(238,228,218,.35);\nborder-radius: 3px;");
  tempLabel -> show();
  endL -> hide();

  QLabel * animation_lbl = new QLabel();
  animation_lbl -> setText(endL -> text());
  animation_lbl -> setStyleSheet(endL -> styleSheet()); // copy endl's appearance to animation_lbl
  animation_lbl -> setFont(endL -> font()); // copy endl's font to animation_lbl
  animation_lbl -> setAlignment(endL -> alignment());

  this -> layout() -> addWidget(animation_lbl); // add animation_lbl to layout
  animation_lbl -> show(); // show animation_lbl

  QPropertyAnimation * animation = new QPropertyAnimation(animation_lbl, "geometry");
  animation -> setDuration(100);
  animation -> setStartValue(startL -> geometry()); // start geometry = startL geometry
  animation -> setEndValue(endL -> geometry()); // start geometry = endL geometry
  animation -> start();

  connect(animation, & QPropertyAnimation::finished, this, [animation_lbl, endL, tempLabel] {
    // when animation ended , delete tempLabel and animation_lbl , then show endl label
    delete animation_lbl;
    delete tempLabel;
    endL -> show();
  });

}

void MainWindow::keyPressEvent(QKeyEvent * event) {

  // handle keyboard event
  if (ui -> frame -> isVisible() == 0) {

    switch (event -> key()) {
    case Qt::Key_D:
      goRight();
      MainWindow::StartNewPoint();
      break;

    case Qt::Key_S:

      goDown();
      MainWindow::StartNewPoint();
      break;

    case Qt::Key_A:
      goLeft();
      MainWindow::StartNewPoint();
      break;
    case Qt::Key_W:
      goTop();
      MainWindow::StartNewPoint();
      break;

    }
  }

}

//generate_random_index returns random number in range 0 to 15
int generate_random_index() {

  int lower = 0, upper = 15;

  int num = (rand() %
    (upper - lower + 1)) + lower;

  for (int nCounter = 0; nCounter <= 16; nCounter++) {
    if (nCounter == num) {
      if (numbers_in_Table[nCounter] != NULL) {
        return 17;
      }
    }
  }
  return num;

}

// setScore updates score
void MainWindow::setScore(int score, int target_index) {

  if (score > 0) {
    ResizeLabelAnimation(target_index);
    TotalScore = TotalScore + score;

    char score_str[20];
    char phr[40] = "Score : ";
    snprintf(score_str, 20, "%d", TotalScore);

    strcat(phr, score_str);
    ui -> Score_lbl -> setText(QString(phr)); // update score label in ui

  }
}

// update grids' label
void MainWindow::setValue_on_Gridindex(int index, int value, int source_index) {

  if (value != -1) {
    numbers_in_Table[index] = value;
  } else
    numbers_in_Table[index] = NULL;

  if (source_index == -1)
    set_Config_onLabel_byIndex(indexToQLabel(index), index, -1);
  else
    set_Config_onLabel_byIndex(indexToQLabel(index), index, 1);

}

void MainWindow::goRight() {

  int mlplyRow;

  for (mlplyRow = 1; mlplyRow <= 4; mlplyRow++) { // each row

    int r_lastIndex = (mlplyRow * 4) - 4; // last index
    int r_firstIndex = (mlplyRow * 4) - 1;// first index

    for (int rowCounter = r_firstIndex; rowCounter >= r_lastIndex; rowCounter--) { // first index to last index
      int min = 100, max = 0, maC = -1;
      if (rowCounter <= r_firstIndex) {
        for (rowCounter; numbers_in_Table[rowCounter] != NULL and numbers_in_Table[rowCounter + 1] == NULL and rowCounter < r_firstIndex; rowCounter++) {

          setValue_on_Gridindex(rowCounter + 1, numbers_in_Table[rowCounter], rowCounter);
          setValue_on_Gridindex(rowCounter, -1, rowCounter);

          if (rowCounter + 1 > max)
            max = rowCounter + 1;
          if (maC == -1)
            min = rowCounter;
          maC++;

        }
        if (max != 0)
          AnimationC(min, max);

      }
    }
    for (int x = r_lastIndex; x < r_firstIndex; x++) {
      if (numbers_in_Table[x] == numbers_in_Table[x + 1] and numbers_in_Table[x] != NULL and numbers_in_Table[x] % 2 == 0 and numbers_in_Table[x] > 0) {
        setScore(2 * numbers_in_Table[x], x + 1);
        setValue_on_Gridindex(x + 1, 2 * numbers_in_Table[x], x);
        setValue_on_Gridindex(x, -1, x);
        for (int rowCounter = r_lastIndex; rowCounter < r_firstIndex; rowCounter++) {
          if (numbers_in_Table[rowCounter] != NULL and numbers_in_Table[rowCounter + 1] == NULL) {
            setValue_on_Gridindex(rowCounter + 1, numbers_in_Table[rowCounter], rowCounter);
            setValue_on_Gridindex(rowCounter, -1, rowCounter);

          }

        }
        x = x + 1;

      }
    }

  }

}

void MainWindow::goLeft() {

  int mlplyRow;
  for (mlplyRow = 1; mlplyRow <= 4; mlplyRow++) {

    int l_lastIndex = (mlplyRow * 4) - 1;
    int l_firstIndex = (mlplyRow * 4) - 4;

    for (int rowCounter = l_firstIndex; rowCounter <= l_lastIndex; rowCounter++) {
      int min = 100, max = 0, maC = -1;
      if (rowCounter >= l_firstIndex) {
        for (rowCounter; numbers_in_Table[rowCounter] != NULL and numbers_in_Table[rowCounter - 1] == NULL and rowCounter > l_firstIndex; rowCounter--) {

          setValue_on_Gridindex(rowCounter - 1, numbers_in_Table[rowCounter], rowCounter);
          setValue_on_Gridindex(rowCounter, -1, rowCounter);

          if (rowCounter < min)
            min = rowCounter;
          if (maC == -1)
            max = rowCounter;
          maC++;

        }

        if (max != 0)
          AnimationC(max, min - 1);

      }
    }
    for (int x = l_lastIndex; x > l_firstIndex; x--) {
      if (numbers_in_Table[x] == numbers_in_Table[x - 1] and numbers_in_Table[x] != NULL and numbers_in_Table[x] % 2 == 0 and numbers_in_Table[x] > 0) {
        setScore(2 * numbers_in_Table[x], x - 1);
        setValue_on_Gridindex(x - 1, 2 * numbers_in_Table[x], x);
        setValue_on_Gridindex(x, -1, x);
        for (int rowCounter = l_lastIndex; rowCounter > l_firstIndex; rowCounter--) {
          if (numbers_in_Table[rowCounter] != NULL and numbers_in_Table[rowCounter - 1] == NULL) {
            setValue_on_Gridindex(rowCounter - 1, numbers_in_Table[rowCounter], rowCounter);
            setValue_on_Gridindex(rowCounter, -1, rowCounter);

          }

        }
        x = x - 1;

      }

    }
  }

}

void MainWindow::goDown() {

  for (int mlplyCol = 1; mlplyCol <= 4; mlplyCol++) {
    int b_lastIndex = mlplyCol - 1;
    int b_firstIndex = mlplyCol + 11;
    int min = 0, max = 0, miC = 0;

    for (int x = b_firstIndex; x >= b_lastIndex; x = x - 4) {

      for (int y = x; y < b_firstIndex; y = y + 4) {
        if (numbers_in_Table[y + 4] == NULL and numbers_in_Table[y] != NULL) {
          if (y > max)
            max = y;
          if (miC == 0)
            min = y;
          miC++;

          setValue_on_Gridindex(y + 4, numbers_in_Table[y], y);
          setValue_on_Gridindex(y, -1, y);
        }

      }
      if (max != 0) {
        AnimationC(min, max + 4);
      }

    }

    for (int q = b_lastIndex; q < b_firstIndex; q = q + 4) {

      if (numbers_in_Table[q] == numbers_in_Table[q + 4] and numbers_in_Table[q] != NULL and numbers_in_Table[q] > 0 and numbers_in_Table[q] % 2 == 0) {
        setScore(2 * numbers_in_Table[q], q + 4);
        setValue_on_Gridindex(q + 4, 2 * numbers_in_Table[q], q);
        setValue_on_Gridindex(q, -1, q);
        for (int y = b_lastIndex; y < b_firstIndex; y = y + 4) {
          if (numbers_in_Table[y + 4] == NULL and numbers_in_Table[y] != NULL) {
            setValue_on_Gridindex(y + 4, numbers_in_Table[y], y);
            setValue_on_Gridindex(y, -1, y);
          }

        }
        q = q + 4;

      }
    }

  }

}

void MainWindow::goTop() {

  for (int mlplyCol = 1; mlplyCol <= 4; mlplyCol++) {
    int t_firstIndex = mlplyCol - 1;
    int t_lastIndex = mlplyCol + 11;
    int min = 100, max = 0;

    for (int p = t_firstIndex; p <= t_lastIndex; p += 4) {
      for (int q = p; q > t_firstIndex; q = q - 4) {
        if (numbers_in_Table[q] != NULL and numbers_in_Table[q - 4] == NULL) {
          if (q > max) max = q;

          if (min > q) min = q;

          setValue_on_Gridindex(q - 4, numbers_in_Table[q], q);
          setValue_on_Gridindex(q, -1, q);

        }

      }
      if (max != 0) {
        AnimationC(max, min - 4);
      }

    }
    for (int x = t_lastIndex; x > t_firstIndex; x = x - 4) {
      if (numbers_in_Table[x] == numbers_in_Table[x - 4] and numbers_in_Table[x] > 0 and numbers_in_Table[x] % 2 == 0) {
        setScore(2 * numbers_in_Table[x], x - 4);
        setValue_on_Gridindex(x - 4, 2 * numbers_in_Table[x], x);
        setValue_on_Gridindex(x, -1, x);
        for (int q = t_lastIndex; q > t_firstIndex; q = q - 4) {
          if (numbers_in_Table[q] != NULL and numbers_in_Table[q - 4] == NULL) {

            setValue_on_Gridindex(q - 4, numbers_in_Table[q], q);
            setValue_on_Gridindex(q, -1, q);

          }

        }
        x = x - 4;
      }

    }

  }

}
void MainWindow::set_Config_onLabel_byIndex(QLabel * target_lbl, int index, int isCreate) {

  char numStr[4];

  if (numbers_in_Table[index] != NULL) {
    snprintf(numStr, 4, "%d", numbers_in_Table[index]);
    if (isCreate == -1) // check if it was the first time a item created
      CreateLabelAnimation(index);
    target_lbl -> setText(QString(numStr));
    switch (numbers_in_Table[index]) {
    // Points' style
    case 2:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #eee4da;color: #776e65");
      break;

    case 4:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #ede0c8;color: #776e65");
      break;

    case 8:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #f2b179;color: #f9f6f2");
      break;

    case 16:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #f59563;color: #f9f6f2");
      break;

    case 32:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #ff775c;color: #f9f6f2");
      break;
    case 64:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #e64c2e;color: #f9f6f2");
      break;
    case 128:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #ede291;color: #f9f6f2;font-size: 38px");
      break;
    case 256:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #fce130;color: #f9f6f2;font-size: 38px");
      break;
    case 512:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #ffdb4a;color: #f9f6f2;font-size: 38px");
      break;
    case 1024:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #f0b922;color: #f9f6f2;font-size: 28px");
      break;
    case 2048:
      target_lbl -> setStyleSheet("border-radius: 3px;background-color: #fad74d;color: #f9f6f2;font-size: 28px");
      break;

    }

  } else {
    target_lbl -> setText(QString(" "));
    target_lbl -> setStyleSheet("background-color: rgba(238,228,218,.35);border-radius: 3px;");
  }

}

void MainWindow::on_pushButton_clicked() {

}

// check if an item of array reach 2048
int checkWin() {

  for (int i = 0; i <= 15; i++) {

    if (numbers_in_Table[i] == 2048) {
      return 1;
    }
  }
  return 0;

}

// check if a empty index exist
int checkLose() {
  for (int i = 0; i <= 15; i++) {
    if (numbers_in_Table[i] == NULL) {
      return 0;
    }
  }
  return 1;

}

void MainWindow::ClearScore() {

  TotalScore = 0;
  ui -> Score_lbl -> setText(QString("Score : 0"));
}

void MainWindow::NewGame() {
  // clear numbers_in_Table items
  for (int i = 0; i <= 15; i++) {
    setValue_on_Gridindex(i, -1, i);
    numbers_in_Table[i] = NULL;
  }

  ClearScore();
  time_t t;
  srand((unsigned) time(&t));

  StartNewPoint();
  StartNewPoint();
}

void MainWindow::OpenMenu(int won) {
    ui -> frame -> setVisible(true);
    if (won)
        ui->status_label->setText(QString("You Won"));
    else
        ui->status_label->setText(QString("Game Over!"));


}

int MainWindow::StartNewPoint() {


  if (checkWin()){
      OpenMenu(1);
      return 1;
  }

  if (checkLose()) {
    OpenMenu(0);
    return 1;
  }

  char randIndex_str[8];
  int randIndex = generate_random_index();

  while (randIndex == 17) {
    randIndex = generate_random_index();
  }

  itoa(randIndex, randIndex_str, 10);

  setValue_on_Gridindex(randIndex, 2, -1);

}

void MainWindow::on_newGame_btn_clicked() {
  NewGame();

}

void MainWindow::on_try_again_btn_clicked() {
  ui -> frame -> setVisible(false);
  NewGame();
}
