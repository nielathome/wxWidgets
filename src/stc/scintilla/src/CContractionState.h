// <NLOG>
// Scintilla source code edit control
// Copyright 1998-2007 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef CCONTRACTIONSTATE_H
#define CCONTRACTIONSTATE_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

template<class T>
class SparseVector;

/**
 */
class ContractionState : public VContractionState {
	// These contain 1 element for every document line.
	RunStyles *visible;
	RunStyles *expanded;
	RunStyles *heights;
	SparseVector<const char *> *foldDisplayTexts;
	Partitioning *displayLines;
	int linesInDocument;

	void EnsureData();

	bool OneToOne() const {
		// True when each document line is exactly one display line so need for
		// complex data structures.
		return visible == 0;
	}

public:
	ContractionState();
	virtual ~ContractionState();

	void Clear() override;

	int LinesInDoc() const override;
	int LinesDisplayed() const override;
	int DisplayFromDoc( int lineDoc ) const override;
	int DisplayLastFromDoc( int lineDoc ) const override;
	int DocFromDisplay( int lineDisplay ) const override;

	void InsertLine( int lineDoc );
	void InsertLines( int lineDoc, int lineCount ) override;
	void DeleteLine( int lineDoc );
	void DeleteLines( int lineDoc, int lineCount ) override;

	bool GetVisible( int lineDoc ) const override;
	bool SetVisible( int lineDocStart, int lineDocEnd, bool isVisible ) override;
	bool HiddenLines() const override;

	const char *GetFoldDisplayText( int lineDoc ) const override;
	bool SetFoldDisplayText( int lineDoc, const char *text ) override;

	bool GetExpanded( int lineDoc ) const override;
	bool SetExpanded( int lineDoc, bool isExpanded ) override;
	bool GetFoldDisplayTextShown( int lineDoc ) const override;
	int ContractedNext( int lineDocStart ) const override;

	int GetHeight( int lineDoc ) const override;
	bool SetHeight( int lineDoc, int height ) override;

	void ShowAll() override;
	void Check() const;
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
