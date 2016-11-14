#pragma once

#include <d3d11.h>

extern ID3D11Device* mDevice;
extern ID3D11DeviceContext* mDeviceContext;

namespace buffer{

	template <typename T>
	void CreateStructuredBuffer(T* data, int nrOfElements, ID3D11UnorderedAccessView** mUAVBuffer)
	{

		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(T) * nrOfElements;
		cbDesc.Usage = D3D11_USAGE_DEFAULT;
		cbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		cbDesc.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = data;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		ID3D11Buffer* uavBuffer;

		HRESULT hr = mDevice->CreateBuffer(&cbDesc, &InitData, &uavBuffer);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = nrOfElements;
		uavDesc.Buffer.Flags = 0;

		hr = mDevice->CreateUnorderedAccessView(uavBuffer, &uavDesc, mUAVBuffer);

	}

	template <typename T>
	T* ReadStructuredBuffer(ID3D11UnorderedAccessView* mUAVBuffer)
	{

		ID3D11Resource* res;
		D3D11_MAPPED_SUBRESOURCE mapSubRes;

		mUAVBuffer->GetResource(&res);

		mDeviceContext->Map(res, 0, D3D11_MAP_READ_WRITE, 0, &mapSubRes);
		mDeviceContext->Unmap(res, 0);

		return (T*)(mapSubRes.pData);

	}

	template <typename T>
	void CreateConstantBuffer(T* data, ID3D11Buffer** cBuffer)
	{

		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(T);
		cbDesc.Usage = D3D11_USAGE_DEFAULT;
		cbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		cbDesc.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = data;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		mDevice->CreateBuffer(&cbDesc, &InitData, cBuffer);

	}

}

