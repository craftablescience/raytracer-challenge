#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QShortcut>
#include <QStyle>
#include <QToolBar>
#include "window.hpp"

rt_window::rt_window(QWidget* parent)
        : QMainWindow(parent) {
    this->setWindowTitle("raytracer_gui");
    this->setWindowIcon(QIcon(":/logo.png"));
    this->setMinimumSize(300, 300);

    auto* fileMenu = this->menuBar()->addMenu(tr("File"));
    fileMenu->addAction(this->style()->standardIcon(QStyle::SP_FileIcon), "New", [&] {
        this->new_file();
    });
    fileMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogOpenButton), "Open", [&] {
        this->open_file();
    });
    fileMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save", [&] {
        this->save_file(false);
    });
    fileMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogSaveButton), "Save As", [&] {
        this->save_file(true);
    });
    fileMenu->addSeparator();
    fileMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogCloseButton), "Exit", [&] {
        this->close();
    });

    auto* helpMenu = this->menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogHelpButton), "About Me", [&] {
        QMessageBox::about(this, tr("About"), "raytracer_gui\n\nmade by craftablescience 2023");
    });
    helpMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogHelpButton), "About Qt", [&] {
        QMessageBox::aboutQt(this);
    });
}

void rt_window::closeEvent(QCloseEvent* event) {
    if (!this->is_modified())
        return;

    auto r = this->ask_for_save();
    if (r == QMessageBox::Cancel) {
        event->ignore();
    } else if (r == QMessageBox::Save) {
        this->save_file();
    }
}

void rt_window::new_file() {}

void rt_window::open_file() {
    /*
    if (!ask_save())
        return;

    auto file =
            QFileDialog::getOpenFileName(this, tr("Open VTF"), QString(), "Valve Texture Format (*.vtf);;All files (*.*)");
    if (file.isEmpty())
        return;

    if (!document()->load_file(file.toUtf8().data())) {
        QMessageBox::warning(this, tr("Error"), tr("Could not open file!"));
    }
    */
}

void rt_window::save_file(bool saveAs /*= false*/) const {
    /*
    if (!document()->dirty())
        return;

    // Ask for a save directory if there's no active file
    if (document()->path().empty() || saveAs) {
        auto name = QFileDialog::getSaveFileName(this, tr("Save as"), QString(), "Valve Texture File (*.vtf)");
        if (name.isEmpty())
            return;
        document()->set_path(name.toUtf8().data());
    }

    if (!document()->save()) {
        QMessageBox::warning(
                this, "Could not save file!", fmt::format(FMT_STRING("Failed to save file: {}"), vlGetLastError()).c_str(),
                QMessageBox::Ok);
        return;
    }

    unmark_modified();
    */
}

void rt_window::mark_modified() {
    auto title = windowTitle();
    if (title.endsWith("*"))
        return;
    setWindowTitle(title + "*");
}

void rt_window::unmark_modified() {
    auto title = this->windowTitle();
    if (!title.endsWith('*'))
        return;
    title.remove(title.length() - 1, 1);
    this->setWindowTitle(title);
}

bool rt_window::is_modified() const {
    return this->windowTitle().endsWith('*');
}

int rt_window::ask_for_save() {
    auto* msgBox = new QMessageBox(QMessageBox::Icon::Question, tr("Quit without saving?"),
                                   tr("You have unsaved changes. Would you like to save?"),
                                   QMessageBox::NoButton, this);
    msgBox->addButton(QMessageBox::Save);
    msgBox->addButton(QMessageBox::Cancel);
    msgBox->addButton(QMessageBox::Discard);
    return msgBox->exec();
}
