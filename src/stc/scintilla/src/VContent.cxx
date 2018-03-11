//
// <NLOG>
// Copyright 2017 by Niel Clausen
// The License.txt file describes the conditions under which this software may be distributed.
//

#include <vector>

#include <string.h>

#include "ILexer.h"
#include "Scintilla.h"

#include "Platform.h"
#include "SplitVector.h"
#include "Partitioning.h"
#include "RunStyles.h"
#include "CellBuffer.h"
#include "CCellBuffer.h"
#include "CharClassify.h"
#include "Decoration.h"
#include "PerLine.h"
#include "CPerLine.h"
#include "CaseFolder.h"
#include "Document.h"



/*-----------------------------------------------------------------------
 * Document
 -----------------------------------------------------------------------*/

void Document::VIndicatorFillRange( vint_t indicator, vint_t start, vint_t size, vint_t value )
{
	decorations.SetCurrentIndicator( indicator );
	decorations.FillRange( start, value, size );
}


void Document::VTextChanged( vint_t origNumLines, vint_t delLength, vint_t insertLength )
{
	enteredModification++;

	const vint_t pos{ 0 };
	if( delLength != 0 )
	{
		NotifyModified(
			DocModification(
				SC_MOD_BEFOREDELETE | SC_PERFORMED_USER,
				pos, delLength,
				0, 0 ) );
		ModifiedAt( pos );
		NotifyModified(
			DocModification(
				SC_MOD_DELETETEXT | SC_PERFORMED_USER, //| (startSequence ? SC_STARTACTION : 0),
				pos, delLength,
				-origNumLines, nullptr /*text*/ ) );
	}

	if( insertLength != 0 )
	{
		NotifyModified(
			DocModification(
				SC_MOD_BEFOREINSERT | SC_PERFORMED_USER,
				pos, insertLength,
				0, 0 /*s*/ ) );
		ModifiedAt( pos );
		NotifyModified(
			DocModification(
				SC_MOD_INSERTTEXT | SC_PERFORMED_USER, // | (startSequence ? SC_STARTACTION : 0),
				pos, insertLength,
				LinesTotal(), nullptr /*text*/ ) );
	}

	enteredModification--;
}



/*-----------------------------------------------------------------------
 * Public
 -----------------------------------------------------------------------*/

// standard ("non-virtual") content
struct StdContent : public VContent
{
	void Release( void ) override {
		delete this;
	}

	VCellBuffer * SCI_METHOD GetCellBuffer( void ) override {
		return new CellBuffer;
	}

	void SCI_METHOD ReleaseCellBuffer( VCellBuffer * cell_buffer ) override {
		delete cell_buffer;
	}

	VLineMarkers * SCI_METHOD GetLineMarkers( void ) override {
		auto ret{ new LineMarkers() };
		intrusive_ptr_add_ref( ret );
		return ret;
	}

	VLineLevels * SCI_METHOD GetLineLevels( void ) override {
		auto ret{ new LineLevels() };
		intrusive_ptr_add_ref( ret );
		return ret;
	}

	VLineState * SCI_METHOD GetLineState( void ) override {
		auto ret{ new LineState() };
		intrusive_ptr_add_ref( ret );
		return ret;
	}

	VLineAnnotation * SCI_METHOD GetLineMargin( void ) override {
		auto ret{ new LineAnnotation() };
		intrusive_ptr_add_ref( ret );
		return ret;
	}

	VLineAnnotation * SCI_METHOD GetLineAnnotation( void ) override {
		auto ret{ new LineAnnotation() };
		intrusive_ptr_add_ref( ret );
		return ret;
	}
};


content_ptr_t MakeStdContent( void )
{
	content_ptr_t content{ new StdContent };
	intrusive_ptr_add_ref( content );
	return content;
}
