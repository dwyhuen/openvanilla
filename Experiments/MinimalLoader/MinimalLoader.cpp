#include "MinimalLoader.h"

extern "C" {
	// defined in OVIMPhoneticStatic.cpp for the time being
	OVModule *OVGetModuleFromLibrary(int idx);
}

static OVInputMethodContext* g_inputMethodContext = 0;
static OVInputMethod* g_inputMethod = 0;
static MLBuffer* g_buffer = 0;
static MLCandidate* g_candidate = 0;
static MLService* g_service = 0;
static MLDictionary* g_dictionary = 0;

void MLInitialize()
{
    g_buffer = new MLBuffer;
    g_candidate = new MLCandidate;
    g_service = new MLService;
    g_dictionary = new MLDictionary;

    g_inputMethod = (OVInputMethod*)OVGetModuleFromLibrary(0);
    if (!g_inputMethod) {
        murmur("failed to get input method module");
        abort();
    }
    
    if (!g_inputMethod->initialize(g_dictionary, g_service, "/tmp/")) {
        murmur("failed to initialize input method module");
        abort();
    }
        
    g_inputMethodContext = g_inputMethod->newContext();
    if (!g_inputMethodContext) {
        murmur("failed to create an input method context");
        abort();
    }
}

void MLStart()
{
    g_inputMethodContext->start(g_buffer, g_candidate, g_service);
}

void MLCancel()
{
    g_inputMethodContext->clear();
    g_inputMethodContext->end();
    g_buffer->clear();
    g_candidate->hide()->clear();
}

void MLEnd()
{
    g_inputMethodContext->clear();
    g_inputMethodContext->end();
    g_buffer->clear();
    g_candidate->hide()->clear();
}

bool MLKey(int code)
{
    MLKeyCode keyCode(code);
    int r = g_inputMethodContext->keyEvent(&keyCode, g_buffer, g_candidate, g_service);
	return r ? true : false;
}

bool MLIsCommited()
{
    return g_buffer->isCommited();
}

string MLCommitedString()
{
    return g_buffer->bufferString();
}

string MLComposingBuffer()
{
    return g_buffer->bufferString();
}

string MLCandidatesString()
{
    return g_candidate->candidatesString();
}
