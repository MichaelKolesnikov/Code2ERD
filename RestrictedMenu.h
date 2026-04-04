#pragma once
#include <QMenu>

class RestrictedMenu : public QMenu
{
public:
    RestrictedMenu(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};
