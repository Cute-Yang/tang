#pragma once
#include "ElaDialog.h"
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "ElaToolButton.h"

namespace tang {
namespace client {
class WorkspaceRenameFile : public ElaDialog {
    Q_OBJECT
private:
    size_t file_index;
public:
    // a better method is using friend keyword!
    ElaText* previous_filename_key;
    ElaText* previous_filename_value;

    ElaText*     current_filename_key;
    ElaLineEdit* current_filename_line_edit;

    ElaToolButton* ok_button;
    ElaToolButton* cancle_button;

public:
    WorkspaceRenameFile(QWidget* parent = nullptr);
    ~WorkspaceRenameFile();
    void display();

    void set_file_index(size_t index);
    size_t get_file_index() const;
};
}   // namespace client
}   // namespace tang