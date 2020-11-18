#pragma once

#include <QWidget>
#include <QHash>
#include "splititem.h"

class QVBoxLayout;

class SplitView : public QWidget
{
    Q_OBJECT

    public:
        Q_INVOKABLE explicit SplitView(QWidget *parent = nullptr);
        void createFirst();
        SplitItem* splitItem(QWidget* w) const;

    protected:
        virtual QDialog* createDialog(QWidget* parent = 0) const;
        virtual SplitView* createView() const;
        virtual QWidget* createWidget();
        virtual void onItemSplit(SplitItem* item, SplitItem* newitem);
        virtual void onItemDestroyed(const SplitItem* item);
        virtual void onItemCreated(SplitItem* item);

    private:
        QHash<QWidget*, SplitItem*> m_items;
        QVBoxLayout* m_layout;

    friend class SplitItem;
};

