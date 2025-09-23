#pragma once
#include "util.h"
#include <QAbstractListModel>
#include <QStandardItemModel>
#include <QString>
#include <span>

namespace tang {
namespace client {

// define the file type

using data_item_type = RemoteFileInfo;
// show the workspace,the top level folder!
using WorkSpaceListViewModel = QStandardItemModel;

// for list view!
class RemoteFileInfoListViewModel : public QAbstractListModel {
    Q_OBJECT
private:
    std::span<data_item_type> file_infos;

public:
    RemoteFileInfoListViewModel(std::span<data_item_type> file_infos_, QObject* parent = nullptr);

    ~RemoteFileInfoListViewModel();

    void set_file_infos(const std::span<data_item_type>& file_infos_);


    int rowCount(const QModelIndex& index) const override;

    QVariant data(const QModelIndex& index, int role) const override;
};

// for table
class RemoteFileInfoViewModel : public QAbstractTableModel {
    Q_OBJECT
private:
    std::span<data_item_type> file_infos;
    QStringList               header;

public:
    RemoteFileInfoViewModel(QStringList header_, std::span<data_item_type> file_infos_ = {},
                            QObject* parent = nullptr);
    ~RemoteFileInfoViewModel() {}
    void set_file_infos(const std::span<data_item_type>& file_infos_);

    RemoteFileInfo& get_file_info(size_t i);
        

protected:
    int      rowCount(const QModelIndex& parent) const override;
    int      columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
};



class RemoteWorkspaceInfoModel : public QAbstractListModel {
    Q_OBJECT
private:
    // only keep a view is all we need!
    std::span<QString> workspace_names;

public:
    RemoteWorkspaceInfoModel(std::span<QString> workspace_names_ = {}, QObject* parent = nullptr);

    ~RemoteWorkspaceInfoModel();

    int rowCount(const QModelIndex& index) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    void set_workspace_names(std::span<QString> workspace_names_);

    QString get_item(size_t index) { return workspace_names[index]; }
};

}   // namespace client
}   // namespace tang