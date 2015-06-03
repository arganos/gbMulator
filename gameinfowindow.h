#ifndef GAMEINFOWINDOW_H
#define GAMEINFOWINDOW_H

#include <QDialog>

namespace Ui {
class GameInfoWindow;
}

class GameInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameInfoWindow(QWidget *parent = 0);
    ~GameInfoWindow();

private:
    Ui::GameInfoWindow *ui;
};

#endif // GAMEINFOWINDOW_H
