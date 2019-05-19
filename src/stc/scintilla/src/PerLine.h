// <NLOG>
// Scintilla source code edit control
/** @file PerLine.h
 ** Manages data associated with each line of the document
 **/
// Copyright 1998-2009 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef PERLINE_H
#define PERLINE_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

class VLineMarkers : public PerLine, public VLifeTime {
protected:
	void Release( void ) override {
		delete this;
	}

public:
	virtual int MarkValue(int line) = 0;
	virtual int MarkerNext(int lineStart, int mask) const = 0;
	virtual int AddMark(int line, int marker, int lines) = 0;
	virtual void MergeMarkers(int pos) = 0;
	virtual bool DeleteMark(int line, int markerNum, bool all) = 0;
	virtual void DeleteMarkFromHandle(int markerHandle) = 0;
	virtual int LineFromHandle(int markerHandle) = 0;
};

class VLineLevels : public PerLine, public VLifeTime {
protected:
	void Release( void ) override {
		delete this;
	}

public:
	virtual void ExpandLevels(int sizeNew=-1) = 0;
	virtual void ClearLevels() = 0;
	virtual int SetLevel(int line, int level, int lines) = 0;
	virtual int GetLevel(int line) const = 0;
};

class VLineState : public PerLine, public VLifeTime {
protected:
	void Release( void ) override {
		delete this;
	}

public:
	virtual int SetLineState(int line, int state) = 0;
	virtual int GetLineState(int line) = 0;
	virtual int GetMaxLineState() const = 0;
};

class VLineAnnotation : public PerLine, public VLifeTime {
protected:
	void Release( void ) override {
		delete this;
	}

public:
	virtual bool MultipleStyles(int line) const = 0;
	virtual int Style(int line) const = 0;
	virtual const char *Text(int line) const = 0;
	virtual const unsigned char *Styles(int line) const = 0;
	virtual void SetText(int line, const char *text) = 0;
	virtual void ClearAll() = 0;
	virtual void SetStyle(int line, int style) = 0;
	virtual void SetStyles(int line, const unsigned char *styles) = 0;
	virtual int Length(int line) const = 0;
	virtual int Lines(int line) const = 0;
};

class VLineTabstops : public PerLine {
public:
	virtual bool ClearTabstops(int line) = 0;
	virtual bool AddTabstop(int line, int x) = 0;
	virtual int GetNextTabstop(int line, int x) const = 0;
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
