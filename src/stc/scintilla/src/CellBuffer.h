// Scintilla source code edit control
/** @file CellBuffer.h
 ** Manages the text of the document.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef CELLBUFFER_H
#define CELLBUFFER_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

// Interface to per-line data that wants to see each line insertion and deletion
class PerLine {
public:
	virtual ~PerLine() {}
	virtual void Init() {}
	virtual void InsertLine(int /* line */ ) {}
	virtual void RemoveLine(int /* line */ ) {}
};

enum actionType { insertAction, removeAction, startAction, containerAction };

/**
 * Actions are used to store all the information required to perform one undo/redo step.
 */
class Action {
public:
	actionType at;
	int position;
	char *data;
	int lenData;
	bool mayCoalesce;

	Action();
	~Action();
	void Create(actionType at_, int position_=0, const char *data_=0, int lenData_=0, bool mayCoalesce_=true);
	void Destroy();
	void Grab(Action *source);
};

/**
 * Holder for an expandable array of characters that supports undo and line markers.
 * Based on article "Data Structures in a Bit-Mapped Text Editor"
 * by Wilfred J. Hansen, Byte January 1987, page 183.
 */
class VCellBuffer {
public:
	/// Retrieving positions outside the range of the buffer works and returns 0
	virtual char CharAt(int position) const = 0;
	virtual void GetCharRange(char *buffer, int position, int lengthRetrieve) const = 0;
	virtual char StyleAt(int position) const = 0;
	virtual void GetStyleRange(unsigned char *buffer, int position, int lengthRetrieve) const = 0;
	virtual const char *BufferPointer() = 0;
	virtual const char *RangePointer(int position, int rangeLength) = 0;
	virtual int GapPosition() const = 0;

	virtual int Length() const = 0;
	virtual void Allocate(int newSize) = 0;
	virtual int GetLineEndTypes() const = 0;
	virtual void SetLineEndTypes(int utf8LineEnds_) = 0;
	virtual bool ContainsLineEnd(const char *s, int length) const = 0;
	virtual void SetPerLine(PerLine *pl) = 0;
	virtual int Lines() const = 0;
	virtual int LineStart(int line) const = 0;
	virtual int LineFromPosition( int pos ) const = 0;
	virtual void InsertLine(int line, int position, bool lineStart) = 0;
	virtual void RemoveLine(int line) = 0;
	virtual const char *InsertString(int position, const char *s, int insertLength, bool &startSequence) = 0;

	/// Setting styles for positions outside the range of the buffer is safe and has no effect.
	/// @return true if the style of a character is changed.
	virtual bool SetStyleAt(int position, char styleValue) = 0;
	virtual bool SetStyleFor(int position, int length, char styleValue) = 0;

	virtual const char *DeleteChars(int position, int deleteLength, bool &startSequence) = 0;

	virtual bool IsReadOnly() const = 0;
	virtual void SetReadOnly(bool set) = 0;

	/// The save point is a marker in the undo stack where the container has stated that
	/// the buffer was saved. Undo and redo can move over the save point.
	virtual void SetSavePoint() = 0;
	virtual bool IsSavePoint() const = 0;

	virtual void TentativeStart() = 0;
	virtual void TentativeCommit() = 0;
	virtual bool TentativeActive() const = 0;
	virtual int TentativeSteps() = 0;

	virtual bool SetUndoCollection(bool collectUndo) = 0;
	virtual bool IsCollectingUndo() const = 0;
	virtual void BeginUndoAction() = 0;
	virtual void EndUndoAction() = 0;
	virtual void AddUndoAction(int token, bool mayCoalesce) = 0;
	virtual void DeleteUndoHistory() = 0;

	/// To perform an undo, StartUndo is called to retrieve the number of steps, then UndoStep is
	/// called that many times. Similarly for redo.
	virtual bool CanUndo() const = 0;
	virtual int StartUndo() = 0;
	virtual const Action &GetUndoStep() const = 0;
	virtual void PerformUndoStep() = 0;
	virtual bool CanRedo() const = 0;
	virtual int StartRedo() = 0;
	virtual const Action &GetRedoStep() const = 0;
	virtual void PerformRedoStep() = 0;
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
