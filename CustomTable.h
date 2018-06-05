#ifndef CUSTOMTABLE_H
#define CUSTOMTABLE_H

#include <QTableWidget>
#include <QList>
#include <QPair>

/** The CustomTable class inherits the QTableWidget class in order
 * to allow the user to dynamically resize the column widths.
 */
class CustomTable : public QTableWidget
{
    Q_OBJECT

public:
    /** Basic constructor.
     */
    explicit CustomTable(QWidget *parent = Q_NULLPTR);

    virtual ~CustomTable();

private:
    Q_DISABLE_COPY(CustomTable)

public:
    /** Inserts a column into the table at the specified column number
     * and repopulate the list.
     * \param column The column number to insert at.
     */
    void insertColumn(qint32 column);

    /** Removes a column from the table at the specified column number
     * kand repopulate the list.
     * \param column The column number to remove.
     */
    void removeColumn(qint32 column);

    /** Set the width of the specified column, then adjust the values for each
     * column in the list.
     * \param column The column to set the width of.
     * \param width The new width of the column.
     */
    void setColumnWidth(qint32 column, qint32 width);

    /** Clear the table and clear the list.
     */
    void clear();

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    //! Flag that indicates that the selected column can be resized.
    bool canResize;
    //! Flag to indicate when the mouse button has been pressed.
    bool isMousePressed;
    //! List containing the start & end values of each column.
    QList<QPair<qint32, qint32>> startEndList;
    //! The selected column that will be adjusted.
    qint32 columnSelected;

    /** Adjust the start & end values for each column.
     */
    void adjustStartEndValues();

    /** Clear the list and repopulate the start & end values for each column.
     */
    void repopulateList();
};

#endif // CUSTOMTABLE_H
