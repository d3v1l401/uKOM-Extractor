
// zlibistream.h
//
// Implements compression and decompression on streams
//
//
// ZSequentialStreamBase , base class for the implementaion classes
//
// Implementation classes for ISequentialStream interfaces, constrained to
// either writing or reading. Template class T needs to provide implementaion
// for overridable member functions FillBuffer and EmptyBuffer.
//
// template<class T, int bufferSize=16384> class ZReadSequentialStreamImpl
// template<class T, int bufferSize=16384> class ZWriteSequentialStreamImpl
//
//
// Implementation of read and write sequential streams based on implementation
// classes above. These classe overrides the FillBuffer and EmptyBuffer by
// reading from or writing to a user supplied stream.
//
// template<int bufferSize=16384> class ZReadSequentialStreamT
// template<int bufferSize=16384> class ZWriteSequentialStreamT
//
// Convenience typedef's for stackbased classes (no AddRef(), Release() or QI)
//
// typedef CComObjectStack<ZWriteSequentialStreamT<> > ZWriteSequentialStream;
// typedef CComObjectStack<ZReadSequentialStreamT<> > ZReadSequentialStream;
//

#ifndef __ZLIBISTREAM_H_
#define __ZLIBISTREAM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// compression/decompression based on zlib

#include <zlib.h>


class ZSequentialStreamBase
{
protected:
	z_stream m_zstm;

public:
	ZSequentialStreamBase()
	{
		memset(&m_zstm,0,sizeof(z_stream));
	}
};

template<class T, int bufferSize=16384>
class ZReadSequentialStreamImpl : public ZSequentialStreamBase, 
			public ISequentialStream
{
public:
	enum { BUFFER_SIZE = bufferSize };
	BYTE m_zbuffer[BUFFER_SIZE];
	ULONG m_zbufLen;
	ULONG m_zcurPos;

	ZReadSequentialStreamImpl() : ZSequentialStreamBase(),m_zbufLen(0),m_zcurPos(0)
	{
	}

	int ZlibInit()
	{
		return inflateInit(&m_zstm); 
	}

	int ZlibCleanup()
	{
		return inflateEnd(&m_zstm);
	}

	STDMETHOD(Read)(void *pv, ULONG cb, ULONG *pcbRead)
	{
		HRESULT hr = S_OK;

		m_zstm.next_out = (Bytef*) pv;
		m_zstm.avail_out = (uInt) cb;

		T* pT = static_cast<T*>(this);

		if (m_zcurPos == m_zbufLen)
		{
			pT->FillBuffer(m_zbuffer, BUFFER_SIZE, &m_zbufLen);
			m_zcurPos = 0;
		}

		int err = Z_OK;
		do
		{
			m_zstm.next_in = (Bytef*) &m_zbuffer[m_zcurPos];
			m_zstm.avail_in = (uInt) (m_zbufLen - m_zcurPos);

			if (m_zstm.avail_in == 0)
			{
				hr = S_FALSE;
				break;
			}

			err = inflate(&m_zstm, Z_SYNC_FLUSH);
			if (err != Z_OK)
				return E_FAIL;

			if (m_zstm.avail_out == 0)
			{
				m_zcurPos = m_zstm.next_in - (Bytef*) &m_zbuffer[0];
				break;
			}
		
			ULONG zcb=0;
			hr = pT->FillBuffer(m_zbuffer, BUFFER_SIZE, &m_zbufLen);
			m_zcurPos=0;
		} while (m_zstm.avail_out > 0);

		if (pcbRead)
			*pcbRead = cb - m_zstm.avail_out;
		return hr;
	}

	STDMETHOD(Write)(void const *pv, ULONG cb, ULONG *pcbWritten)
	{
		ATLTRACENOTIMPL(_T("ZReadSequentialStreamImpl::Write"));
	}


	// overrideable

	HRESULT FillBuffer(void *pv, ULONG cb, ULONG *pcbRead)
	{
		if (pcbRead)
			*pcbRead = 0;
		return S_FALSE;
	}
};



template<class T, int bufferSize=16384>
class ZWriteSequentialStreamImpl : public ZSequentialStreamBase, 
			public ISequentialStream
{
public:
	enum { BUFFER_SIZE = bufferSize };
	BYTE m_zbuffer[BUFFER_SIZE];
	ULONG m_zbufLen;
	ULONG m_streamSize;

	ZWriteSequentialStreamImpl() : ZSequentialStreamBase(),m_zbufLen(0),m_streamSize(0)
	{
	}

	int ZlibInit()
	{
		return deflateInit(&m_zstm, Z_DEFAULT_COMPRESSION); 
	}

	int ZlibCleanup()
	{
		int err = deflate(&m_zstm, Z_FINISH);
        ATLASSERT(err == Z_STREAM_END);

		T* pT = static_cast<T*>(this);

		m_streamSize += m_zbufLen;

		if ( m_zbufLen > 0 )
		{	
			ULONG zcb;
			HRESULT hr = pT->EmptyBuffer(m_zbuffer, m_zbufLen, &zcb);
			m_zbufLen=0;
		}

		err = deflateEnd(&m_zstm);

		return err;
	}

	STDMETHOD(Read)(void *pv, ULONG cb, ULONG *pcbRead)
	{
		ATLTRACENOTIMPL(_T("ZWriteSequentialStreamImpl::Read"));
	}

	STDMETHOD(Write)(void const *pv, ULONG cb, ULONG *pcbWritten)
	{
		HRESULT hr = S_OK;

		m_zstm.next_in = (Bytef*) pv;
		m_zstm.avail_in = (uInt) cb;

		T* pT = static_cast<T*>(this);

		if (m_zbufLen == BUFFER_SIZE)
		{
			ULONG zcb;
			pT->EmptyBuffer(m_zbuffer, BUFFER_SIZE, &zcb);
			m_streamSize += m_zbufLen;
			m_zbufLen=0;
		}


		int err = Z_OK;
		do
		{
			m_zstm.next_out = (Bytef*) &m_zbuffer[m_zbufLen];
			m_zstm.avail_out = (uInt) (BUFFER_SIZE - m_zbufLen);

			err = deflate(&m_zstm, Z_SYNC_FLUSH);
			if (err != Z_OK)
				return E_FAIL;

			if (m_zstm.avail_in == 0)
			{
				m_zbufLen = m_zstm.next_out - (Bytef*) &m_zbuffer[0];
				break;
			}
		
			ULONG zcb;
			hr = pT->EmptyBuffer(m_zbuffer, BUFFER_SIZE, &zcb);
			m_streamSize += m_zbufLen;
			m_zbufLen=0;
		} while (m_zstm.avail_in > 0);

		if (pcbWritten)
			*pcbWritten = cb;
		return hr;
	}


	// overrideable

	HRESULT EmptyBuffer(void const *pv, ULONG cb, ULONG *pcbWritten)
	{
		if (pcbWritten)
			*pcbWritten = cb;
		return S_OK;
	}
};

template<int bufferSize=16384>
class ZReadSequentialStreamT : public ZReadSequentialStreamImpl<ZReadSequentialStreamT, bufferSize>
{
public:
	typedef ZReadSequentialStreamImpl<ZReadSequentialStreamT, bufferSize> baseClass;

	CComPtr<ISequentialStream> m_spStm;
	bool m_bInitialized;

	ZReadSequentialStreamT(ISequentialStream* p=NULL) : baseClass(),m_bInitialized(false)
	{
	}

	HRESULT FinalConstruct() { return S_OK; }
	void FinalRelease() { Cleanup(); }


	// Initializer and cleanup

	bool Initialize(ISequentialStream* p)
	{
		if (m_bInitialized)
		{
			// cannot be initialized twice
			ATLASSERT(false);
			return false;
		}
		m_bInitialized = true;
		m_spStm = p;
		int err = ZlibInit();
		return err == Z_OK;
	}

	bool Cleanup()
	{
		if (!m_bInitialized)
			return true;

		m_bInitialized = false;
		int err = ZlibCleanup();
		m_spStm.Release();
		return err == Z_OK;
	}

	// overrides

	HRESULT FillBuffer(void *pv, ULONG cb, ULONG *pcbRead)
	{
		ATLASSERT(m_spStm != NULL);
		if (!m_spStm)
		{
			if (pcbRead)
				*pcbRead = 0;
			return E_FAIL;
		}
		return m_spStm->Read(pv, cb, pcbRead);
	}
};

typedef CComObjectStack<ZReadSequentialStreamT<> > ZReadSequentialStream;

template<int bufferSize=16384>
class ZWriteSequentialStreamT : public ZWriteSequentialStreamImpl<ZWriteSequentialStreamT, bufferSize>
{
public:
	typedef ZWriteSequentialStreamImpl<ZWriteSequentialStreamT, bufferSize> baseClass;

	CComPtr<IStream> m_spStm;
	bool m_bInitialized;

	ZWriteSequentialStreamT() : baseClass(),m_bInitialized(false)
	{
	}

	HRESULT FinalConstruct() { return S_OK; }
	void FinalRelease() { Cleanup(); }

	// Initializer and cleanup

	bool Initialize(IStream* p)
	{
		if (m_bInitialized)
		{
			// cannot be initialized twice
			ATLASSERT(false);
			return false;
		}
		m_bInitialized = true;
		m_spStm = p;
		int err = ZlibInit();
		return err == Z_OK;
	}

	bool Cleanup()
	{
		if (!m_bInitialized)
			return true;

		m_bInitialized = false;
		int err = ZlibCleanup();
		if (m_spStm)
		{
			ULARGE_INTEGER ul;
			ul.QuadPart = __int64(m_streamSize);
			m_spStm->SetSize(ul);
			m_spStm->Commit(STGC_DEFAULT);
		}
		m_spStm.Release();
		return err == Z_OK;
	}

	// overrides

	HRESULT EmptyBuffer(void const *pv, ULONG cb, ULONG *pcbWritten)
	{
		ATLASSERT(m_spStm != NULL);
		if (!m_spStm)
		{
			if (pcbWritten)
				*pcbWritten = 0;
			return E_FAIL;
		}
		return m_spStm->Write(pv, cb, pcbWritten);
	}
};

typedef CComObjectStack<ZWriteSequentialStreamT<> > ZWriteSequentialStream;

#endif //__ZLIBISTREAM_H_