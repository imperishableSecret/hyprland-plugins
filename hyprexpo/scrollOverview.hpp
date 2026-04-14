#pragma once

#define WLR_USE_UNSTABLE

#include "globals.hpp"
#include <hyprland/src/desktop/DesktopTypes.hpp>
#include <hyprland/src/render/Framebuffer.hpp>
#include <hyprland/src/helpers/AnimatedVariable.hpp>
#include <hyprland/src/event/EventBus.hpp>
#include <hyprland/src/helpers/signal/Signal.hpp>
#include <vector>

using namespace Render;

#include "IOverview.hpp"

class CMonitor;

class CScrollOverview : public IOverview {
  public:
    CScrollOverview(PHLWORKSPACE startedOn_, bool swipe = false);
    virtual ~CScrollOverview();

    virtual void render();
    virtual void damage();
    virtual void onDamageReported();
    virtual void onPreRender();

    virtual void setClosing(bool closing);

    virtual void resetSwipe();
    virtual void onSwipeUpdate(double delta);
    virtual void onSwipeEnd();

    // close without a selection
    virtual void close();
    virtual void selectHoveredWorkspace();

    virtual void fullRender();

  private:
    void   redrawWorkspace(PHLWORKSPACE w, bool forcelowres = false);
    void   redrawAll(bool forcelowres = false);
    void   onWorkspaceChange();
    void   highlightHoverDebug();
    void   moveViewportWorkspace(bool up);

    bool   damageDirty              = false;
    size_t viewportCurrentWorkspace = 0;

    struct SWindowImage {
        PHLWINDOWREF            pWindow;
        SP<IFramebuffer>        fb;
        bool                    highlight = false;
        UP<CHyprSignalListener> windowCommit;
        Vector2D                lastWindowPosition, lastWindowSize;
    };

    void redrawWindowImage(SP<SWindowImage>);

    struct SWorkspaceImage {
        PHLWORKSPACE                  pWorkspace;
        CBox                          box;
        std::vector<SP<SWindowImage>> windowImages;
    };

    SP<IFramebuffer>                 backgroundFb;
    SP<IFramebuffer>                 floatingFb;

    Vector2D                         lastMousePosLocal = Vector2D{};

    PHLWINDOWREF                     closeOnWindow;

    std::vector<SP<SWorkspaceImage>> images;
    SP<SWorkspaceImage>              imageForWorkspace(PHLWORKSPACE w);

    PHLWORKSPACE                     startedOn;

    PHLANIMVAR<float>                scale;
    PHLANIMVAR<Vector2D>             viewOffset;

    bool                             closing = false;

    CHyprSignalListener             mouseMoveHook;
    CHyprSignalListener             mouseButtonHook;
    CHyprSignalListener             touchMoveHook;
    CHyprSignalListener             touchDownHook;
    CHyprSignalListener             mouseAxisHook;
    CHyprSignalListener             windowOpenHook;

    bool                             swipe             = false;
    bool                             swipeWasCommenced = false;

    friend class CScrollOverviewPassElement;
};

inline std::unique_ptr<CScrollOverview> g_pScrollOverview;
