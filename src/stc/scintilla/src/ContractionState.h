// <NLOG>
// Scintilla source code edit control
/** @file ContractionState.h
 ** Manages visibility of lines for folding and wrapping.
 **/
// Copyright 1998-2007 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef CONTRACTIONSTATE_H
#define CONTRACTIONSTATE_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

/**
 */
class VContractionState : public VLifeTime {
protected:
	void Release( void ) override {
		delete this;
	}

public:
	virtual void Clear() = 0;

	virtual int LinesInDoc() const = 0;
	virtual int LinesDisplayed() const = 0;
	virtual int DisplayFromDoc(int lineDoc) const = 0;
	virtual int DisplayLastFromDoc(int lineDoc) const = 0;
	virtual int DocFromDisplay(int lineDisplay) const = 0;

	virtual void InsertLines(int lineDoc, int lineCount) = 0;
	virtual void DeleteLines(int lineDoc, int lineCount) = 0;

	virtual bool GetVisible(int lineDoc) const = 0;
	virtual bool SetVisible(int lineDocStart, int lineDocEnd, bool isVisible) = 0;
	virtual bool HiddenLines() const = 0;

	virtual const char *GetFoldDisplayText(int lineDoc) const = 0;
	virtual bool SetFoldDisplayText(int lineDoc, const char *text) = 0;

	virtual bool GetExpanded(int lineDoc) const = 0;
	virtual bool SetExpanded(int lineDoc, bool isExpanded) = 0;
	virtual bool GetFoldDisplayTextShown(int lineDoc) const = 0;
	virtual int ContractedNext(int lineDocStart) const = 0;

	virtual int GetHeight(int lineDoc) const = 0;
	virtual bool SetHeight(int lineDoc, int height) = 0;

	virtual void ShowAll() = 0;
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
