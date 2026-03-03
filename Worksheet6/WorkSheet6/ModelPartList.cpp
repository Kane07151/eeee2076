#include "ModelPartList.h"
#include "ModelPart.h"

ModelPartList::ModelPartList( const QString& data, QObject* parent )
    : QAbstractItemModel(parent)
{
    Q_UNUSED(data);
    // 只有两列：Part | Visible?
    rootItem = new ModelPart( { tr("Part"), tr("Visible?") } );
}

ModelPartList::~ModelPartList()
{
    delete rootItem;
}

int ModelPartList::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

int ModelPartList::rowCount( const QModelIndex& parent ) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    ModelPart* parentItem = nullptr;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    if (!parentItem) return 0;
    return parentItem->childCount();
}

QModelIndex ModelPartList::index( int row, int column, const QModelIndex& parent ) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ModelPart* parentItem = nullptr;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    if (!parentItem) return QModelIndex();

    ModelPart* childItem = parentItem->child(row);
    if (!childItem) return QModelIndex();

    return createIndex(row, column, childItem);
}

QModelIndex ModelPartList::parent( const QModelIndex& index ) const
{
    if (!index.isValid())
        return QModelIndex();

    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    if (!childItem) return QModelIndex();

    ModelPart* parentItem = childItem->parentItem();
    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

QVariant ModelPartList::data( const QModelIndex& index, int role ) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    if (!item) return QVariant();

    return item->data(index.column());
}

bool ModelPartList::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole)
        return false;

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    if (!item) return false;

    item->set(index.column(), value);

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags ModelPartList::flags( const QModelIndex& index ) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    // Visible 列允许编辑（可双击改 true/false）
    if (index.column() == 1)
        f |= Qt::ItemIsEditable;

    return f;
}

QVariant ModelPartList::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

ModelPart* ModelPartList::getRootItem()
{
    return rootItem;
}

QModelIndex ModelPartList::appendChild( QModelIndex& parent, const QList<QVariant>& data )
{
    ModelPart* parentPart = nullptr;
    QModelIndex parentIndex;

    if (parent.isValid()) {
        parentPart = static_cast<ModelPart*>(parent.internalPointer());
        parentIndex = parent;
    } else {
        parentPart = rootItem;
        parentIndex = QModelIndex();
    }

    if (!parentPart) return QModelIndex();

    const int insertRow = parentPart->childCount();

    beginInsertRows(parentIndex, insertRow, insertRow);

    ModelPart* childPart = new ModelPart(data, parentPart);
    parentPart->appendChild(childPart);

    endInsertRows();

    return createIndex(insertRow, 0, childPart);
}
