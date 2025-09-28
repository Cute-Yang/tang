#pragma once
#include "ElaDialog.h"
#include "ElaText.h"
#include "ElaToolButton.h"


namespace tang {
namespace client {
class WorkspaceDeleteFile : public ElaDialog {
    Q_OBJECT
private:
    // contain the dir and file
    QString delete_filename;

public:
    ElaText*       hint_text;
    ElaToolButton* ok_button;
    ElaToolButton* cancle_button;

public:
    WorkspaceDeleteFile(QWidget* parent = nullptr);
    ~WorkspaceDeleteFile();
    void display();
    void set_delete_filename(const QString& filename);

    const QString& get_delete_fielname() const;
};
}   // namespace client
}   // namespace tang