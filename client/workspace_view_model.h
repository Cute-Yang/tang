#pragma once
#include <QAbstractListModel>
#include <QStandardItemModel>
#include <QString>
#include <cstdint>

namespace tang {
namespace client {

// define the file type
enum class SimpleFileType : uint32_t {
    k_foler  = 0,
    k_pdf    = 1,
    k_word   = 2,
    k_excel  = 3,
    k_ppt    = 4,
    k_txt    = 5,
    k_image  = 6,
    k_python = 7,
    k_cpp    = 8,
    k_others = 9,
    count
};

struct RemoteFileInfo {
    QString        file_name;
    size_t         file_size;
    SimpleFileType file_type;
    QString        modify_time;

    static constexpr size_t get_field_size() { return 4; }
};


using data_item_type = RemoteFileInfo;
using data_type      = QList<RemoteFileInfo>;
// show the workspace,the top level folder!
using WorkSpaceListViewModel = QStandardItemModel;

// for list view!
class RemoteFileInfoListViewModel : public QAbstractListModel {
    Q_OBJECT
private:
    std::shared_ptr<data_type> file_infos;


public:
    RemoteFileInfoListViewModel(std::shared_ptr<data_type> file_infos_, QObject* parent = nullptr);

    ~RemoteFileInfoListViewModel();

    void set_file_infos(const std::shared_ptr<data_type>& file_infos_);

    int rowCount(const QModelIndex& index) const override;

    QVariant data(const QModelIndex& index, int role) const override;
};

// for table
class RemoteFileInfoViewModel : public QAbstractTableModel {
    Q_OBJECT
private:
    std::shared_ptr<data_type> file_infos;
    QStringList                header;

public:
    RemoteFileInfoViewModel(QStringList header_, std::shared_ptr<data_type> file_infos_,
                            QObject* parent = nullptr);
    ~RemoteFileInfoViewModel() {}
    void set_file_infos(const std::shared_ptr<data_type>& file_infos_);


protected:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;


    QVariant data(const QModelIndex& index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
};



class RemoteWorkspaceInfoModel : public QAbstractListModel {
    Q_OBJECT
private:
    QList<QString> workspace_names;

public:
    RemoteWorkspaceInfoModel(QList<QString> workspace_names_, QObject* parent = nullptr);

    ~RemoteWorkspaceInfoModel();

    int rowCount(const QModelIndex& index) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    void set_workspace_names(const QList<QString>& workspace_names_);

    void set_workspace_names(const QString* workspace_names_ptr, size_t len);
};

}   // namespace client
}   // namespace tang