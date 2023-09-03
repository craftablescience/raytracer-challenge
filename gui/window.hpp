#pragma once

#include <array>
#include <QMainWindow>

class rt_window : public QMainWindow {
    Q_OBJECT;
public:
    explicit rt_window(QWidget* parent = nullptr);

    void new_file();
    void open_file();
    void save_file(bool saveAs = false) const;

protected:
    void closeEvent(QCloseEvent* event) override;

    void mark_modified();
    void unmark_modified();
    [[nodiscard]] bool is_modified() const;
    [[nodiscard]] int ask_for_save();

    enum class actions : int {
        NEW = 0,
        OPEN,
        SAVE,
        SAVE_AS,
        EXIT,

        SIZE,
    };
    QShortcut* shortcut(actions act) {
        return this->shortcuts[static_cast<int>(act)];
    }
private:
    std::array<QShortcut*, static_cast<int>(actions::SIZE)> shortcuts{nullptr};
};
