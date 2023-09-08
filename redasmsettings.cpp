#include "redasmsettings.h"
#include <QStandardPaths>
#include <QFontDatabase>
#include <QApplication>
#include <QDir>

#define CURRENT_WINDOW_GEOMETRY "window_geometry_2"
#define CURRENT_WINDOW_STATE    "window_state_2"

QByteArray REDasmSettings::m_defaultstate;

REDasmSettings::REDasmSettings(QObject *parent): QSettings(QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)).absoluteFilePath("redasm.conf"), QSettings::IniFormat, parent) { }

bool REDasmSettings::restoreState(QMainWindow *mainwindow)
{
    m_defaultstate = mainwindow->saveState(); // Keep default state

    if(this->contains(CURRENT_WINDOW_GEOMETRY))
        mainwindow->restoreGeometry(this->value(CURRENT_WINDOW_GEOMETRY).toByteArray());

    if(this->contains(CURRENT_WINDOW_STATE))
    {
        mainwindow->restoreState(this->value(CURRENT_WINDOW_STATE).toByteArray());
        return true;
    }

    return false;
}

void REDasmSettings::defaultState(QMainWindow *mainwindow) { mainwindow->restoreState(m_defaultstate); }

void REDasmSettings::saveState(const QMainWindow *mainwindow)
{
    this->setValue(CURRENT_WINDOW_GEOMETRY, mainwindow->saveGeometry());
    this->setValue(CURRENT_WINDOW_STATE, mainwindow->saveState());
}

void REDasmSettings::clearRecentFiles() { this->setValue("recent_files", QStringList()); }
QStringList REDasmSettings::recentFiles() const { return this->value("recent_files").toStringList(); }

void REDasmSettings::updateRecentFiles(const QString &s)
{
    QStringList recents = this->recentFiles();
    recents.removeAll(s); // Remove duplicates
    recents.prepend(s);

    while(recents.length() > MAX_RECENT_FILES)
        recents.removeLast();

    this->setValue("recent_files", recents);
}

QString REDasmSettings::currentTheme() const { return this->value("selected_theme", "light").toString(); }
QFont REDasmSettings::currentFont() const { return this->value("selected_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>(); }

int REDasmSettings::currentFontSize() const
{
    int size = qApp->font().pixelSize();

    if(size == -1)
        size = QFontMetrics{qApp->font()}.height();

    return this->value("selected_font_size", size).toInt();
}

void REDasmSettings::changeTheme(const QString& theme) { this->setValue("selected_theme", theme.toLower()); }
void REDasmSettings::changeFont(const QFont &font) { this->setValue("selected_font", font);  }
void REDasmSettings::changeFontSize(int size) { this->setValue("selected_font_size", size); }

QFont REDasmSettings::font()
{
    REDasmSettings settings;
    QFont f = settings.currentFont();

    if(f.styleHint() == QFont::Monospace)
    {
        f.setFamily("Monospace"); // Force Monospaced font
        f.setStyleHint(QFont::TypeWriter);
    }

    f.setPointSize(settings.currentFontSize());
    return f;
}
