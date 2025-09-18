#include "workspace_view_model.h"

namespace tang {
namespace client {


constexpr size_t FileTypeCount = static_cast<size_t>(SimpleFileType::count);
static std::array<QString, FileTypeCount> file_type_strings = {
    "文件夹", "PDF", "Word", "Excel", "PPT", "txt", "image", "python", "c++", "其他文件"};

static std::array<QString, FileTypeCount> file_type_icon_strings = {
    ":/icons/images/better_folder.svg",
    ":/icons/images/pdf.svg",
    ":/icons/images/word.svg",
    ":/icons/images/excel.svg",
    ":/icons/images/ppt.svg",
    ":/icons/images/txt.svg",
    ":/icons/images/img_file.svg",
    ":/icons/images/py.svg",
    ":/icons/images/cpp.svg",
    ":/icons/images/unknown_file.svg"};

std::array<QIcon, FileTypeCount> initialize_file_icons() {
    std::array<QIcon, FileTypeCount> icons;
    // icons.resize(file_type_icon_strings.size());
    for (size_t i = 0; i < file_type_icon_strings.size(); ++i) {
        icons[i] = QIcon(file_type_icon_strings[i]);
    }
    return icons;
}

static std::array<QIcon, FileTypeCount> file_type_icons = initialize_file_icons();

RemoteFileInfoViewModel::RemoteFileInfoViewModel(QStringList                headers_,
                                                 std::shared_ptr<data_type> file_infos_,
                                                 QObject*                   parent)
    : header(headers_)
    , file_infos(std::move(file_infos_))
    , QAbstractTableModel(parent) {}

void RemoteFileInfoViewModel::set_file_infos(const std::shared_ptr<data_type>& new_file_infos) {
    if (new_file_infos != nullptr) {
        file_infos = new_file_infos;
    }
}

int RemoteFileInfoViewModel::rowCount(const QModelIndex& parent) const {
    if (file_infos != nullptr) {
        return file_infos->count();
    }
    return 0;
}

int RemoteFileInfoViewModel::columnCount(const QModelIndex& parent) const {
    return header.count();
}



QVariant RemoteFileInfoViewModel::data(const QModelIndex& index, int role) const {
    int   row      = index.row();
    int   column   = index.column();
    auto& row_data = file_infos->at(row);
    // maybe invalid?
    if (role == Qt::DisplayRole) {
        if (column == 0) {
            // file_name
            return row_data.file_name;
        } else if (column == 1) {
            //file type
            return file_type_strings[static_cast<size_t>(row_data.file_type)];
        } else if (column == 2) {
            //file size
            return row_data.file_size;
        } else if (column == 3) {
            //change time
            return row_data.modify_time;
        }
    } else if (role == Qt::DecorationRole && column == 0) {
        //display icon at first column!
        return file_type_icons[static_cast<size_t>(row_data.file_type)];
    }
    return QVariant();
}

QVariant RemoteFileInfoViewModel::headerData(int section, Qt::Orientation orientation,
                                             int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return header[section];
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

RemoteFileInfoListViewModel::RemoteFileInfoListViewModel(std::shared_ptr<data_type> file_infos_,
                                                         QObject*                   parent)
    : QAbstractListModel(parent)
    , file_infos(std::move(file_infos_)) {}

RemoteFileInfoListViewModel::~RemoteFileInfoListViewModel() {}


void RemoteFileInfoListViewModel::set_file_infos(const std::shared_ptr<data_type>& file_infos_) {
    if (file_infos_ != nullptr) {
        file_infos = file_infos_;
    }
}
int RemoteFileInfoListViewModel::rowCount(const QModelIndex& index) const {
    Q_UNUSED(index);
    return file_infos->count();
}

QVariant RemoteFileInfoListViewModel::data(const QModelIndex& index, int role) const {
    auto& row_data = file_infos->at(index.row());
    if (role == Qt::DisplayRole) {
        return row_data.file_name;
    } else if (role == Qt::DecorationRole) {
        size_t icon_index = static_cast<size_t>(row_data.file_type);
        return file_type_icons[icon_index];
    }
    return QVariant{};
}


static QIcon workspace_icon = QIcon(":/icons/images/workspace.svg");


RemoteWorkspaceInfoModel::RemoteWorkspaceInfoModel(QList<QString> workspace_names_, QObject* parent)
    : workspace_names(std::move(workspace_names_))
    , QAbstractListModel(parent) {}

RemoteWorkspaceInfoModel::~RemoteWorkspaceInfoModel() {}

int RemoteWorkspaceInfoModel::rowCount(const QModelIndex& index) const {
    return workspace_names.size();
}

QVariant RemoteWorkspaceInfoModel::data(const QModelIndex& index, int role) const {
    int row = index.row();
    if (role == Qt::DisplayRole) {
        return workspace_names[row];
    } else if (role == Qt::DecorationRole) {
        return workspace_icon;
    }
    return QVariant{};
}


void RemoteWorkspaceInfoModel::set_workspace_names(const QString* workspace_names_ptr, size_t len) {
    if (workspace_names_ptr == nullptr || len == 0) {
        return;
    }
    workspace_names.assign(workspace_names_ptr, workspace_names_ptr + len);
}

void RemoteWorkspaceInfoModel::set_workspace_names(const QList<QString>& workspace_names_) {
    workspace_names.assign(workspace_names_.begin(), workspace_names_.end());
}

}   // namespace client
}   // namespace tang