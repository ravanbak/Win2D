// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may
// not use these files except in compliance with the License. You may obtain
// a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

#include "pch.h"
#include "CanvasLinearGradientBrush.h"
#include "CanvasDevice.h"
#include "Gradients.h"

namespace ABI { namespace Microsoft { namespace Graphics { namespace Canvas
{
    using ABI::Windows::UI::Color;
    using namespace ABI::Microsoft::Graphics::Canvas::Numerics;
    using namespace ::Microsoft::WRL::Wrappers;

    ComPtr<CanvasLinearGradientBrush> CanvasLinearGradientBrushManager::CreateNew(
        ICanvasResourceCreator* resourceCreator,
        UINT32 gradientStopCount,
        CanvasGradientStop* gradientStops,
        CanvasEdgeBehavior edgeBehavior,
        CanvasAlphaBehavior alphaBehavior,
        CanvasColorSpace preInterpolationSpace,
        CanvasColorSpace postInterpolationSpace,
        CanvasBufferPrecision bufferPrecision)
    {
        ComPtr<ICanvasDevice> device;
        resourceCreator->get_Device(&device);

        ComPtr<ICanvasDeviceInternal> deviceInternal;
        ThrowIfFailed(device.As(&deviceInternal));

        ComPtr<ID2D1GradientStopCollection1> stopCollection = deviceInternal->CreateGradientStopCollection(
            gradientStopCount,
            gradientStops,
            edgeBehavior,
            preInterpolationSpace,
            postInterpolationSpace,
            bufferPrecision,
            alphaBehavior);

        auto d2dBrush = deviceInternal->CreateLinearGradientBrush(stopCollection.Get());

        auto canvasLinearGradientBrush = Make<CanvasLinearGradientBrush>(
            shared_from_this(),
            d2dBrush.Get());
        CheckMakeResult(canvasLinearGradientBrush);

        return canvasLinearGradientBrush;
    }

    ComPtr<CanvasLinearGradientBrush> CanvasLinearGradientBrushManager::CreateNew(
        ICanvasResourceCreator* resourceCreator,
        ID2D1GradientStopCollection1* stopCollection)
    {
        ComPtr<ICanvasDevice> device;
        resourceCreator->get_Device(&device);

        ComPtr<ICanvasDeviceInternal> deviceInternal;
        ThrowIfFailed(device.As(&deviceInternal));

        auto d2dBrush = deviceInternal->CreateLinearGradientBrush(stopCollection);

        auto canvasLinearGradientBrush = Make<CanvasLinearGradientBrush>(
            shared_from_this(),
            d2dBrush.Get());
        CheckMakeResult(canvasLinearGradientBrush);

        return canvasLinearGradientBrush;
    }

    ComPtr<CanvasLinearGradientBrush> CanvasLinearGradientBrushManager::CreateWrapper(
        ID2D1LinearGradientBrush* resource)
    {
        auto canvasLinearGradientBrush = Make<CanvasLinearGradientBrush>(
            shared_from_this(),
            resource);
        CheckMakeResult(canvasLinearGradientBrush);

        return canvasLinearGradientBrush;
    }

    IFACEMETHODIMP CanvasLinearGradientBrushFactory::CreateSimple(
        ICanvasResourceCreator* resourceCreator,
        ABI::Windows::UI::Color startColor,
        ABI::Windows::UI::Color endColor,
        ICanvasLinearGradientBrush** linearGradientBrush)
    {
        return ExceptionBoundary(
            [&]
            {
                CheckInPointer(resourceCreator);
                CheckAndClearOutPointer(linearGradientBrush);

                ComPtr<ICanvasDevice> canvasDevice;
                resourceCreator->get_Device(&canvasDevice);

                ComPtr<ID2D1GradientStopCollection1> stopCollection =
                    CreateSimpleGradientStopCollection(canvasDevice.Get(), startColor, endColor, CanvasEdgeBehavior::Clamp);

                auto newLinearBrush = GetManager()->Create(resourceCreator, stopCollection.Get());

                ThrowIfFailed(newLinearBrush.CopyTo(linearGradientBrush));
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrushFactory::CreateWithEdgeBehaviorAndAlphaBehavior(
        ICanvasResourceCreator* resourceAllocator,
        UINT32 gradientStopCount,
        CanvasGradientStop* gradientStops,
        CanvasEdgeBehavior edgeBehavior,
        CanvasAlphaBehavior alphaBehavior,
        ICanvasLinearGradientBrush** linearGradientBrush)
    {
        return CreateWithEdgeBehaviorAndInterpolationOptions(
            resourceAllocator,
            gradientStopCount,
            gradientStops,
            edgeBehavior,
            alphaBehavior,
            CanvasColorSpace::Srgb,
            CanvasColorSpace::Srgb,
            CanvasBufferPrecision::Precision8UIntNormalized,
            linearGradientBrush);
    }

    IFACEMETHODIMP CanvasLinearGradientBrushFactory::CreateWithEdgeBehaviorAndInterpolationOptions(
        ICanvasResourceCreator* resourceCreator,
        UINT32 gradientStopCount,
        CanvasGradientStop* gradientStops,
        CanvasEdgeBehavior edgeBehavior,
        CanvasAlphaBehavior alphaBehavior,
        CanvasColorSpace preInterpolationSpace,
        CanvasColorSpace postInterpolationSpace,
        CanvasBufferPrecision bufferPrecision,
        ICanvasLinearGradientBrush** linearGradientBrush)
    {
        return ExceptionBoundary(
            [&]
            {
                CheckInPointer(resourceCreator);
                CheckAndClearOutPointer(linearGradientBrush);

                auto newLinearBrush = GetManager()->Create(
                    resourceCreator,
                    gradientStopCount,
                    gradientStops,
                    edgeBehavior,
                    alphaBehavior,
                    preInterpolationSpace,
                    postInterpolationSpace,
                    bufferPrecision);

                ThrowIfFailed(newLinearBrush.CopyTo(linearGradientBrush));
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrushFactory::CreateRainbow(
        ICanvasResourceCreator* resourceCreator,
        float eldritchness,
        ICanvasLinearGradientBrush** canvasLinearGradientBrush)
    {
        return ExceptionBoundary(
            [&]
            {
                CheckInPointer(resourceCreator);
                CheckAndClearOutPointer(canvasLinearGradientBrush);

                ComPtr<ICanvasDevice> canvasDevice;
                resourceCreator->get_Device(&canvasDevice);

                ComPtr<ID2D1GradientStopCollection1> stopCollection =
                    CreateRainbowGradientStopCollection(canvasDevice.Get(), eldritchness);

                auto newLinearBrush = GetManager()->Create(resourceCreator, stopCollection.Get());

                ThrowIfFailed(newLinearBrush.CopyTo(canvasLinearGradientBrush));
            });
    }

    CanvasLinearGradientBrush::CanvasLinearGradientBrush(
        std::shared_ptr<CanvasLinearGradientBrushManager> manager,
        ID2D1LinearGradientBrush* brush)
        : ResourceWrapper(manager, brush)
    {
        assert(brush != nullptr);
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::get_StartPoint(_Out_ Vector2* value)
    {
        return ExceptionBoundary(
            [&]()
            {
                CheckInPointer(value);
                *value = FromD2DPoint(GetD2DLinearGradientBrush()->GetStartPoint());
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::put_StartPoint(_In_ Vector2 value)
    {
        return ExceptionBoundary(
            [&]()
            {
                GetD2DLinearGradientBrush()->SetStartPoint(ToD2DPoint(value));
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::get_EndPoint(_Out_ Vector2* value)
    {
        return ExceptionBoundary(
            [&]()
            {
                CheckInPointer(value);
                *value = FromD2DPoint(GetD2DLinearGradientBrush()->GetEndPoint());
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::put_EndPoint(_In_ Vector2 value)
    {
        return ExceptionBoundary(
            [&]()
            {
                GetD2DLinearGradientBrush()->SetEndPoint(ToD2DPoint(value));
            });
    }
    
    IFACEMETHODIMP CanvasLinearGradientBrush::get_Stops(UINT32* valueCount, CanvasGradientStop** valueElements)
    {
        return ExceptionBoundary(
            [&]()
            {
                CheckInPointer(valueCount);
                CheckAndClearOutPointer(valueElements);

                auto& resource = GetResource();

                ComPtr<ID2D1GradientStopCollection> stopCollection;
                resource->GetGradientStopCollection(&stopCollection);
                CopyStops(stopCollection, valueCount, valueElements);
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::get_EdgeBehavior(CanvasEdgeBehavior* value)
    {
        return ExceptionBoundary(
            [&]()
            {
                CheckInPointer(value);
                *value = static_cast<CanvasEdgeBehavior>(GetGradientStopCollection()->GetExtendMode());
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::get_AlphaMode(CanvasAlphaBehavior* value)
    {
        return ExceptionBoundary(
            [&]()
            {
                CheckInPointer(value);
                *value = FromD2DColorInterpolation(GetGradientStopCollection()->GetColorInterpolationMode());
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::get_PreInterpolationSpace(CanvasColorSpace* value)
    {
        return ExceptionBoundary(
            [&]()
            {
                CheckInPointer(value);
                *value = static_cast<CanvasColorSpace>(GetGradientStopCollection()->GetPreInterpolationSpace());
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::get_PostInterpolationSpace(CanvasColorSpace* value)
    {
        return ExceptionBoundary(
            [&]()
            {
                CheckInPointer(value);
                *value = static_cast<CanvasColorSpace>(GetGradientStopCollection()->GetPostInterpolationSpace());
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::get_BufferPrecision(CanvasBufferPrecision* value)
    {
        return ExceptionBoundary(
            [&]()
            {
                CheckInPointer(value);
                *value = FromD2DBufferPrecision(GetGradientStopCollection()->GetBufferPrecision());
            });
    }

    IFACEMETHODIMP CanvasLinearGradientBrush::Close()
    {
        return ResourceWrapper::Close();
    }

    ComPtr<ID2D1Brush> CanvasLinearGradientBrush::GetD2DBrush()
    {
        return GetResource();
    }

    ComPtr<ID2D1LinearGradientBrush> CanvasLinearGradientBrush::GetD2DLinearGradientBrush()
    {
        return GetResource();
    }

    ComPtr<ID2D1GradientStopCollection1> CanvasLinearGradientBrush::GetGradientStopCollection()
    {
        auto& resource = GetResource();
        ComPtr<ID2D1GradientStopCollection> stopCollection;
        resource->GetGradientStopCollection(stopCollection.GetAddressOf());
        ComPtr<ID2D1GradientStopCollection1> stopCollection1;
        ThrowIfFailed(stopCollection.As(&stopCollection1));
        return stopCollection1;
    }

    ActivatableClassWithFactory(CanvasLinearGradientBrush, CanvasLinearGradientBrushFactory);
} } } }