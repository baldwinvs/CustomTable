#include "CustomTable.h"

#include <QHeaderView>
#include <QMouseEvent>
#include <QPoint>

CustomTable::CustomTable(QWidget *parent)
    : QTableWidget(parent),
      canResize(false),
      isMousePressed(false),
      columnSelected(-1)
{
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);
    this->setMouseTracking(true);
}

CustomTable::~CustomTable()
{

}

void CustomTable::mouseMoveEvent(QMouseEvent *event)
{
    static QPoint pos;
    static const qint32 tolerance(5);
    pos = event->pos();

    if(!isMousePressed)
    {
        //don't resize the last column
        for(qint32 i = 0; i < startEndList.size() - 1; ++i)
        {
            if((pos.x() < (startEndList[i].second + tolerance)) && (pos.x() > (startEndList[i].second - tolerance)))
            {
                //get the table height
                qint32 tableHeight(this->rowCount() * this->rowHeight(0));

                //if the cursor is within the table and on a column line,
                //change the cursor
                if((pos.y() < tableHeight) && (pos.y() > this->y()))
                {
                    this->setCursor(Qt::SizeHorCursor);
                    columnSelected = i;
                    canResize = true;
                    break;
                }
            }
            else
                canResize = false;
        }

        //if the resize flag isn't set, change to the normal cursor
        if(!canResize)
        {
            this->setCursor(Qt::ArrowCursor);
        }
    }
    else
    {
        if(canResize)
        {
            static qint32 widthLeftColumn(0);
            static qint32 widthRightColumn(0);

            //get the width of the left and right columns relative to the cursor
            widthLeftColumn = pos.x() - startEndList[columnSelected].first;
            widthRightColumn = (startEndList[columnSelected + 1].second - startEndList[columnSelected + 1].first)
                             + (startEndList[columnSelected].second - pos.x());

            //only adjust the width while the left and right widths are greater than
            //the tolerance value
            if((widthLeftColumn > tolerance) && (widthRightColumn > tolerance))
            {
                this->setColumnWidth(columnSelected, widthLeftColumn);
                this->setColumnWidth(columnSelected + 1, widthRightColumn);
            }
        }
    }
}

void CustomTable::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);

    isMousePressed = true;
}

void CustomTable::mouseReleaseEvent(QMouseEvent *event)
{
    //do whatever the QTableWidget does during a mouseReleaseEvent.
    QTableWidget::mouseReleaseEvent(event);

    isMousePressed = false;

    //adjust the values for each column
    adjustStartEndValues();
}

void CustomTable::clear()
{
    QTableWidget::clear();

    //clear the list
    startEndList.clear();
}

void CustomTable::setColumnWidth(qint32 column, qint32 width)
{
    QTableWidget::setColumnWidth(column, width);

    //adjust the values for each column
    adjustStartEndValues();
}

void CustomTable::insertColumn(qint32 column)
{
    QTableWidget::insertColumn(column);

    //clear the list before repopulating
    startEndList.clear();

    repopulateList();
}

void CustomTable::removeColumn(qint32 column)
{
    QTableWidget::removeColumn(column);

    //clear the lsit before repopulating
    startEndList.clear();

    repopulateList();
}

void CustomTable::adjustStartEndValues()
{
    //don't bother the first of the 0th element
    startEndList[0].second = this->columnWidth(0);

    //now adjust the rest of the list
    for(qint32 i = 1; i < startEndList.size(); ++i)
    {
        startEndList[i].first = startEndList[i - 1].second;
        startEndList[i].second = startEndList[i].first + this->columnWidth(i);
    }
}

void CustomTable::repopulateList()
{
    static QPair<qint32, qint32> pair(0,0);

    for(qint32 i = 0; i < this->columnCount(); ++i)
    {
        if(i == 0)
            pair.first = this->pos().x();
        else
            pair.first = startEndList[i - 1].second;

        pair.second = pair.first + this->columnWidth(i);
        startEndList.append(pair);
    }
}
