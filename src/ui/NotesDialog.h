#pragma once

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include "../core/Types.h"

/**
 * @brief NotesDialog - Окно просмотра найденных записок
 */
class NotesDialog : public QDialog {
    Q_OBJECT

public:
    explicit NotesDialog(const QVector<NoteData>& notes, QWidget* parent = nullptr);

private:
    void setupUI();
    void updateNotesDisplay();

private:
    QVector<NoteData> m_notes;
    QListWidget* m_notesList = nullptr;
    QTextEdit* m_noteContent = nullptr;
    QLabel* m_counterLabel = nullptr;

private slots:
    void onNoteSelected(int index);
};