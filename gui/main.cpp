#include <QApplication>
#include "window.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    auto* rtgui = new rt_window{};
    rtgui->show();

    return QApplication::exec();
}
