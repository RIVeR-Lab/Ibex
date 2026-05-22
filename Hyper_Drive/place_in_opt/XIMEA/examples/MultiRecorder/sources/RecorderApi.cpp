/*
MultiRecorder - communication with XIMEA cameras over http, fast recording, remote camera control, multiplatform

Copyright (c) 2023-2025 XIMEA s.r.o.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#include "RecorderApi.h"

#include <cstdio>
#include <iostream>
#include <fstream>

#include <tinyxml.h>

namespace xiRecorderApi
{

//----------------------------------------------------------------------------------------
std::string formatRequest(const std::map<std::string, std::string> &dictParameters)
{
    std::string sRequest = std::string("<") + XI_SRV_TAG_REQUEST;
    for (const auto &el : dictParameters)
    {
        sRequest += std::string(" ") + el.first + "=\"" + el.second + "\"";
    }
    return sRequest + "/>";
}

//----------------------------------------------------------------------------------------
std::string formatResponse(bool bResult, const std::string &sDetails, int *pxiReturnCode, const std::string *psValueType)
{
    std::string sResponse = std::string("<") + XI_SRV_TAG_RESPONSE + " code=\"" + (bResult ? XI_SRV_RESPONSE_CODE_OK : XI_SRV_RESPONSE_CODE_ERROR);
    if (pxiReturnCode != nullptr && *pxiReturnCode >= 0)
        sResponse += std::string("\" xiret=\"") + std::to_string(*pxiReturnCode);
    if (psValueType != nullptr && !psValueType->empty())
        sResponse += std::string("\" valuetype=\"") + *psValueType;
    return sResponse + "\">" + sDetails +"</" + XI_SRV_TAG_RESPONSE + ">";
}

//----------------------------------------------------------------------------------------
void parseXmlResponse(const std::string &sXMLResponse, bool &bResult, std::string &sDetails, int *pxiReturnCode)
{
    bResult = false;
    sDetails.clear();

    TiXmlDocument doc;
    TiXmlElement* pElCmd = nullptr;
    parseXmlResponseToXml(sXMLResponse, bResult, &doc, pElCmd, pxiReturnCode);
    if (pElCmd != nullptr)
    {
        if (pElCmd->FirstChild() != nullptr && pElCmd->FirstChild()->ToText())
            sDetails = pElCmd->FirstChild()->ToText()->ValueStr();
        else
        {
            const char *szDetails = pElCmd->GetText();
            if (szDetails != nullptr)
                sDetails = szDetails;
        }
    }
}

//----------------------------------------------------------------------------------------
void parseXmlResponseToXml(const std::string &sXMLResponse, bool &bResult, TiXmlDocument *pResponseToXmlDoc, TiXmlElement *&pRootResponseElement, int *pxiReturnCode)
{
    bResult = false;
    if (pxiReturnCode != nullptr) *pxiReturnCode = -1;
    if (pResponseToXmlDoc == nullptr)
        return;

    TiXmlBase::SetCondenseWhiteSpace(false);    // to keep newlines in context list, etc

    pResponseToXmlDoc->Parse(sXMLResponse.c_str(), nullptr, TIXML_ENCODING_UTF8);
    pRootResponseElement = pResponseToXmlDoc->FirstChildElement(XI_SRV_TAG_RESPONSE);
    if (pRootResponseElement != nullptr)
    {
        const char *szAttrCode = pRootResponseElement->Attribute("code");
        if (szAttrCode != nullptr)
            bResult = (strcmp(szAttrCode, XI_SRV_RESPONSE_CODE_OK) == 0);
        if (pxiReturnCode != nullptr)
        {
            const char *szAttrXiRet = pRootResponseElement->Attribute("xiret");
            if (szAttrXiRet != nullptr)
                *pxiReturnCode = atoi(szAttrXiRet);
        }
    }
}

//----------------------------------------------------------------------------------------
std::string g_sLogFilename;
void setLogFile(const std::string &sLogFilename)
{
    g_sLogFilename = sLogFilename;

    // and clear it
    if (!g_sLogFilename.empty())
    {
        /*std::ofstream logfile(g_sLogFilename, std::ios_base::trunc); // Open file in truncation mode
        if (!logfile.is_open())
            std::cerr << "Unable to clear the log file!" << std::endl;
        else
            logfile.close();*/

        // redirect cerr to our file
        //std::ofstream *pLog = new std::ofstream(sLogFilename);
        //std::cerr.rdbuf(pLog->rdbuf());

        freopen(sLogFilename.c_str(), "w", stderr);     // this also catches xiApi messages sent to stderr (not on Win32)
    }
}

//----------------------------------------------------------------------------------------
bool hasLogFile()
{
    return !g_sLogFilename.empty();
}

//----------------------------------------------------------------------------------------
void outputStringToStdErr(const std::string &sMessage)
{
    std::cerr << sMessage << std::endl;
    /*
    if (g_sLogFilename.empty())
        std::cerr << sMessage << std::endl;
    else
    {
        std::ofstream logfile;
        logfile.open(g_sLogFilename, std::ios_base::app);
        if (logfile.is_open())
        {
            logfile << sMessage << std::endl;
            logfile.close();
        }
        else
        {
            std::cerr << "Unable to open the log file" << std::endl;

            // next time output to console
            g_sLogFilename.clear();
            std::cerr << sMessage << std::endl;
        }
    }*/
}

//----------------------------------------------------------------------------------------
void outputStringToConsole(const std::string &sMessage)
{
    std::cout << sMessage << std::endl;
}

}   // namespace XiRecorderApi
