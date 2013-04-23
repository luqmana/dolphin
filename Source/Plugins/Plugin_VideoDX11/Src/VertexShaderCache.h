// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#ifndef _VERTEXSHADERCACHE_H
#define _VERTEXSHADERCACHE_H

#include "VertexShaderGen.h"

#include "D3DBase.h"
#include "D3DBlob.h"

#include <map>

namespace DX11 {

class VertexShaderCache
{
public:
	static void Init();
	static void Clear();
	static void Shutdown();
	static bool SetShader(u32 components); // TODO: Should be renamed to LoadShader

	static ID3D11VertexShader* GetActiveShader() { return last_entry->shader; }
	static D3DBlob* GetActiveShaderBytecode() { return last_entry->bytecode; }
	static ID3D11Buffer* &GetConstantBuffer();

	static ID3D11VertexShader* GetSimpleVertexShader();
	static ID3D11VertexShader* GetClearVertexShader();
	static ID3D11InputLayout* GetSimpleInputLayout();
	static ID3D11InputLayout* GetClearInputLayout();

	static bool VertexShaderCache::InsertByteCode(const VERTEXSHADERUID &uid, D3DBlob* bcodeblob);

private:
	struct VSCacheEntry
	{ 
		ID3D11VertexShader* shader;
		D3DBlob* bytecode; // needed to initialize the input layout

		VERTEXSHADERUIDSAFE safe_uid;
		std::string code;

		VSCacheEntry() : shader(NULL), bytecode(NULL) {}
		void SetByteCode(D3DBlob* blob)
		{
			SAFE_RELEASE(bytecode);
			bytecode = blob;
			blob->AddRef();
		}
		void Destroy()
		{
			SAFE_RELEASE(shader);
			SAFE_RELEASE(bytecode);
		}
	};
	typedef std::map<VERTEXSHADERUID, VSCacheEntry> VSCache;
	
	static VSCache vshaders;
	static const VSCacheEntry* last_entry;
	static VERTEXSHADERUID last_uid;
};

}  // namespace DX11

#endif  // _VERTEXSHADERCACHE_H
