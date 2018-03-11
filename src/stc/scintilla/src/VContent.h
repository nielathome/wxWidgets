//
// <NLOG>
// Copyright 2017 by Niel Clausen
// The License.txt file describes the conditions under which this software may be distributed.
//

#ifndef VCONTENT_H
#define VCONTENT_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif


//
// Scintilla uses an integer type for all line numbers, document positions
// and sizes
//
using vint_t = int;

inline vint_t vint_cast( size_t value )
{
	return static_cast<vint_t>(value);
}


	
/*-----------------------------------------------------------------------
 * VLifeTime
 -----------------------------------------------------------------------*/

// Cross DLL safe memory management via reference counting. Requires the Release
// virtual to be implemented in the same DLL that created the derived object. This
// permits a DLL build against a debug heap to work with Release builds of Python.
// The implementation is compatible with, but does not require, boost::intrusive_ptr.
class VLifeTime
{
private:
	unsigned m_RefCount{ 0 };

	virtual void Release( void ) = 0;

	friend inline void intrusive_ptr_add_ref( VLifeTime * object )
	{
		++object->m_RefCount;
	}

	friend inline void intrusive_ptr_release( VLifeTime * object )
	{
		if( --object->m_RefCount == 0 )
			object->Release();
	}

protected:
	virtual ~VLifeTime( void ) {}
};



/*-----------------------------------------------------------------------
 * VObserver
 -----------------------------------------------------------------------*/

 // Interface for observation services; used to notify content providers of key events
// occuring within Scintilla
struct VObserver
{
	// Scintilla -> content (notifications)
	virtual void __stdcall Notify_StartDrawView( void ) {}
	virtual void __stdcall Notify_StartDrawLine( vint_t /*line_no*/ ) {}
};



/*-----------------------------------------------------------------------
 * VContent
 -----------------------------------------------------------------------*/

// Forwards
struct VControl;
class VCellBuffer;
class VContractionState;
class VLineMarkers;
class VLineLevels;
class VLineState;
class VLineAnnotation;

 // A content provider.
class VContent : public VLifeTime, public VObserver
{
private:
	VControl * m_Control{ nullptr };

protected:
	friend class VDocument;
	void SetControl( VControl * control ) {
		m_Control = control;
	}

public:
	VControl * GetControl( void ) const {
		return m_Control;
	}

	// data access services; potentially, accross DLL boundaries
	virtual VCellBuffer * __stdcall GetCellBuffer( void ) = 0;
	virtual void __stdcall ReleaseCellBuffer( VCellBuffer * cell_buffer ) = 0;
	virtual VLineMarkers * __stdcall GetLineMarkers( void ) = 0;
	virtual VLineLevels * __stdcall GetLineLevels( void ) = 0;
	virtual VLineState * __stdcall GetLineState( void ) = 0;
	virtual VLineAnnotation * __stdcall GetLineMargin( void ) = 0;
	virtual VLineAnnotation * __stdcall GetLineAnnotation( void ) = 0;

	// content managed display heights (optional)
	virtual VContractionState * __stdcall GetContractionState( void ) {
		return nullptr;
	}
};


// Content providers are shared objects; the Scintilla code manages the
// reference counts manually.
using content_ptr_t = VContent *;



/*-----------------------------------------------------------------------
 * VControl
 -----------------------------------------------------------------------*/

// Interface for control of a Scintilla Document. Used to provide access to data/operations
// that cannot (easily) be virtualised by any other mechanism. Provides a direct control
// path from a content provider to a Scintilla document,
struct VControl
{
	// content -> Scintilla (control)
	virtual void VIndicatorFillRange( vint_t indicator, vint_t start, vint_t size, vint_t value ) = 0;
	virtual void VTextChanged( vint_t origNumLines, vint_t delLength, vint_t insertLength ) = 0;
};



/*-----------------------------------------------------------------------
 * VDocument
 -----------------------------------------------------------------------*/

// Core interface to "virtualise" the data in a Scintilla Document. This enables content
// providers to furnish text/style data, as well as be informed of, and respond to,
// editor events.
class VDocument : public VControl
{
private:
	// all data is sourced from the content provider
	content_ptr_t m_Content;

protected:
	VCellBuffer *m_CellBuffer;

	VDocument( content_ptr_t content )
		: m_Content{ content },
		  m_CellBuffer{ content->GetCellBuffer() }
	{
		m_Content->SetControl( this );
	}

	~VDocument()
	{
		m_Content->ReleaseCellBuffer( m_CellBuffer );
		intrusive_ptr_release( m_Content );
		m_Content = nullptr;
	}

	content_ptr_t GetContent( void ) {
		return m_Content;
	}

public:
	// VObserver support; pass notifications on to the VContent
	void Notify_StartDrawView( void ) {
		m_Content->Notify_StartDrawView();
	}
	void Notify_StartDrawLine( vint_t line_no ) {
		m_Content->Notify_StartDrawLine( line_no );
	}

	VContractionState * GetContractionState( void ) {
		return m_Content->GetContractionState();
	}

};



/*-----------------------------------------------------------------------
 * Public 
 -----------------------------------------------------------------------*/

// Create a content provider which wraps the Scintilla standard implementation.
content_ptr_t MakeStdContent( void );


#ifdef SCI_NAMESPACE
}
#endif

#endif
