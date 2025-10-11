#include "workspace_view_model.h"


using namespace tang::common;
namespace tang {
namespace client {

QString get_file_size_str(size_t file_size) {
    if (file_size < 1024) {
        return QString("%1(Byte)").arg(file_size);
    } else if (file_size < 1024 * 1024) {
        return QString("%1(Kb)").arg(file_size / 1024.0, 0, 'f', 2);
    } else if (file_size < 1024 * 1024 * 1024) {
        return QString("%1(Mb)").arg(file_size / (1024.0 * 1024.0), 0, 'f', 2);
    } else {
        return QString("%1(Gb)").arg(file_size / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }
}

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


RemoteFileInfoViewModel::RemoteFileInfoViewModel(QStringList               headers_,
                                                 std::span<data_item_type> file_infos_,
                                                 QObject*                  parent)
    : QAbstractTableModel(parent)
    , header(headers_)
    , file_infos(std::move(file_infos_))
    , file_type_icons(initialize_file_icons()) {}

void RemoteFileInfoViewModel::set_file_infos(const std::span<data_item_type>& new_file_infos) {
    file_infos = new_file_infos;
}

int RemoteFileInfoViewModel::rowCount(const QModelIndex& parent) const {
    return file_infos.size();
}

int RemoteFileInfoViewModel::columnCount(const QModelIndex& parent) const {
    return header.count();
}

RemoteFileInfo& RemoteFileInfoViewModel::get_file_info(size_t i) {
    // qDebug() << this->file_infos.size() <<  " " << i;
    if (i >= this->file_infos.size()) {
        throw std::out_of_range("the index is out of range!");
    }
    return this->file_infos[i];
}

void RemoteFileInfoViewModel::notify_remove_item(size_t i) {
    beginRemoveRows(QModelIndex(), i, i);
    endRemoveRows();
}

QVariant RemoteFileInfoViewModel::data(const QModelIndex& index, int role) const {
    int   row      = index.row();
    int   column   = index.column();
    auto& row_data = file_infos[row];
    // maybe invalid?
    if (role == Qt::DisplayRole) {
        if (column == 1) {
            // file_name
            return row_data.file_name;
        } else if (column == 2) {
            // file type
            return file_type_strings[static_cast<size_t>(row_data.file_type)];
        } else if (column == 3) {
            // file size
            return get_file_size_str(row_data.file_size);
        } else if (column == 4) {
            // change time
            return row_data.modify_time;
        }
    } else if (role == Qt::DecorationRole && column == 0) {
        // display icon at first column!
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

RemoteFileInfoListViewModel::RemoteFileInfoListViewModel(std::span<data_item_type> file_infos_,
                                                         QObject*                  parent)
    : QAbstractListModel(parent)
    , file_infos(std::move(file_infos_))
    , file_type_icons(initialize_file_icons()) {}

RemoteFileInfoListViewModel::~RemoteFileInfoListViewModel() {}


void RemoteFileInfoListViewModel::set_file_infos(const std::span<data_item_type>& file_infos_) {
    file_infos = file_infos_;
}

RemoteFileInfo& RemoteFileInfoListViewModel::get_file_info(size_t i) {
    return file_infos[i];
}

void RemoteFileInfoListViewModel::notify_remove_item(size_t i) {
    beginRemoveRows(QModelIndex(), i, i);
    endRemoveRows();
}


int RemoteFileInfoListViewModel::rowCount(const QModelIndex& index) const {
    Q_UNUSED(index);
    return file_infos.size();
}

QVariant RemoteFileInfoListViewModel::data(const QModelIndex& index, int role) const {
    auto& row_data = file_infos[index.row()];
    if (role == Qt::DisplayRole) {
        return row_data.file_name;
    } else if (role == Qt::DecorationRole) {
        size_t icon_index = static_cast<size_t>(row_data.file_type);
        return file_type_icons[icon_index];
    }
    return QVariant{};
}


RemoteWorkspaceInfoModel::RemoteWorkspaceInfoModel(std::span<QString> workspace_names_,
                                                   QObject*           parent)
    : workspace_names(workspace_names_)
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
        QIcon workspace_icon = QIcon(":/icons/images/workspace.svg");
        return workspace_icon;
    }
    return QVariant{};
}

void RemoteWorkspaceInfoModel::set_workspace_names(std::span<QString> workspace_names_) {
    workspace_names = workspace_names_;
}


}   // namespace client
}   // namespace tang