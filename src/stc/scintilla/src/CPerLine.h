// Scintilla source code edit control
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef CPERLINE_H
#define CPERLINE_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

/**
 * This holds the marker identifier and the marker type to display.
 * MarkerHandleNumbers are members of lists.
 */
struct MarkerHandleNumber {
	int handle;
	int number;
	MarkerHandleNumber *next;
};

/**
 * A marker handle set contains any number of MarkerHandleNumbers.
 */
class MarkerHandleSet {
	MarkerHandleNumber *root;

public:
	MarkerHandleSet();
	~MarkerHandleSet();
	int Length() const;
	int MarkValue() const;	///< Bit set of marker numbers.
	bool Contains(int handle) const;
	bool InsertHandle(int handle, int markerNum);
	void RemoveHandle(int handle);
	bool RemoveNumber(int markerNum, bool all);
	void CombineWith(MarkerHandleSet *other);
};

class LineMarkers : public VLineMarkers {
	SplitVector<MarkerHandleSet *> markers;
	/// Handles are allocated sequentially and should never have to be reused as 32 bit ints are very big.
	int handleCurrent;
public:
	LineMarkers() : handleCurrent(0) {
	}
	~LineMarkers();
	void Init() override;
	void InsertLine(int line) override;
	void RemoveLine(int line) override;

	int MarkValue(int line) override;
	int MarkerNext(int lineStart, int mask) const override;
	int AddMark(int line, int marker, int lines) override;
	void MergeMarkers(int pos) override;
	bool DeleteMark(int line, int markerNum, bool all) override;
	void DeleteMarkFromHandle(int markerHandle) override;
	int LineFromHandle(int markerHandle) override;
};

class LineLevels : public VLineLevels {
	SplitVector<int> levels;
public:
	~LineLevels();
	void Init() override;
	void InsertLine(int line) override;
	void RemoveLine(int line) override;

	void ExpandLevels(int sizeNew=-1) override;
	void ClearLevels() override;
	int SetLevel(int line, int level, int lines) override;
	int GetLevel(int line) const override;
};

class LineState : public VLineState {
	SplitVector<int> lineStates;
public:
	LineState() {
	}
	~LineState();
	void Init() override;
	void InsertLine(int line) override;
	void RemoveLine(int line) override;

	int SetLineState(int line, int state) override;
	int GetLineState(int line) override;
	int GetMaxLineState() const override;
};

class LineAnnotation : public VLineAnnotation {
	SplitVector<char *> annotations;
public:
	LineAnnotation() {
	}
	~LineAnnotation();
	void Init() override;
	void InsertLine(int line) override;
	void RemoveLine(int line) override;

	bool MultipleStyles(int line) const override;
	int Style(int line) const override;
	const char *Text(int line) const override;
	const unsigned char *Styles(int line) const override;
	void SetText(int line, const char *text) override;
	void ClearAll() override;
	void SetStyle(int line, int style) override;
	void SetStyles(int line, const unsigned char *styles) override;
	int Length(int line) const override;
	int Lines(int line) const override;
};

typedef std::vector<int> TabstopList;

class LineTabstops : public VLineTabstops {
	SplitVector<TabstopList *> tabstops;
public:
	LineTabstops() {
	}
	~LineTabstops();
	void Init() override;
	void InsertLine(int line) override;
	void RemoveLine(int line) override;

	bool ClearTabstops(int line) override;
	bool AddTabstop(int line, int x) override;
	int GetNextTabstop(int line, int x) const  override;
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
