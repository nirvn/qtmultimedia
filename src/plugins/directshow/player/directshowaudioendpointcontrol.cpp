/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
****************************************************************************/

#include "directshowaudioendpointcontrol.h"

#include "directshowglobal.h"
#include "directshowplayerservice.h"

QT_BEGIN_NAMESPACE

DirectShowAudioEndpointControl::DirectShowAudioEndpointControl(
        DirectShowPlayerService *service, QObject *parent)
    : QAudioOutputSelectorControl(parent)
    , m_service(service)
{
    if (CreateBindCtx(0, &m_bindContext) == S_OK) {
        m_deviceEnumerator = com_new<ICreateDevEnum>(CLSID_SystemDeviceEnum);

        updateEndpoints();

        setActiveOutput(m_defaultEndpoint);
    }
}

DirectShowAudioEndpointControl::~DirectShowAudioEndpointControl()
{
    for (IMoniker *moniker : qAsConst(m_devices))
        moniker->Release();

    if (m_bindContext)
        m_bindContext->Release();

    if (m_deviceEnumerator)
        m_deviceEnumerator->Release();
}

QList<QString> DirectShowAudioEndpointControl::availableOutputs() const
{
    return m_devices.keys();
}

QString DirectShowAudioEndpointControl::outputDescription(const QString &name) const
{
#ifdef __IPropertyBag_INTERFACE_DEFINED__
    QString description;

    if (IMoniker *moniker = m_devices.value(name, 0)) {
        IPropertyBag *propertyBag = nullptr;
        if (SUCCEEDED(moniker->BindToStorage(
                nullptr, nullptr, IID_IPropertyBag, reinterpret_cast<void **>(&propertyBag)))) {
            VARIANT name;
            VariantInit(&name);
            if (SUCCEEDED(propertyBag->Read(L"FriendlyName", &name, nullptr)))
                description = QString::fromWCharArray(name.bstrVal);
            VariantClear(&name);
            propertyBag->Release();
        }
    }

    return description;
#else
    return name.section(QLatin1Char('\\'), -1);
#endif
}

QString DirectShowAudioEndpointControl::defaultOutput() const
{
    return m_defaultEndpoint;
}

QString DirectShowAudioEndpointControl::activeOutput() const
{
    return m_activeEndpoint;
}

void DirectShowAudioEndpointControl::setActiveOutput(const QString &name)
{
    if (m_activeEndpoint == name)
        return;

    if (IMoniker *moniker = m_devices.value(name, 0)) {
        IBaseFilter *filter = nullptr;

        if (moniker->BindToObject(
                m_bindContext,
                nullptr,
                IID_IBaseFilter,
                reinterpret_cast<void **>(&filter)) == S_OK) {
            m_service->setAudioOutput(filter);

            filter->Release();
        }
    }
}

void DirectShowAudioEndpointControl::updateEndpoints()
{
    IMalloc *oleMalloc = nullptr;
    if (m_deviceEnumerator && CoGetMalloc(1, &oleMalloc) == S_OK) {
        IEnumMoniker *monikers = nullptr;

        if (m_deviceEnumerator->CreateClassEnumerator(
                CLSID_AudioRendererCategory, &monikers, 0) == S_OK) {
            for (IMoniker *moniker = nullptr; monikers->Next(1, &moniker, nullptr) == S_OK; moniker->Release()) {
                OLECHAR *string = nullptr;
                if (moniker->GetDisplayName(m_bindContext, nullptr, &string) == S_OK) {
                    QString deviceId = QString::fromWCharArray(string);
                    oleMalloc->Free(string);

                    moniker->AddRef();
                    m_devices.insert(deviceId, moniker);

                    if (m_defaultEndpoint.isEmpty()
                            || deviceId.endsWith(QLatin1String("Default DirectSound Device"))) {
                        m_defaultEndpoint = deviceId;
                    }
                }
            }
            monikers->Release();
        }
        oleMalloc->Release();
    }
}

QT_END_NAMESPACE
