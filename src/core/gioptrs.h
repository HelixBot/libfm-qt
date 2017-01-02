#ifndef GIOPTRS_H
#define GIOPTRS_H
// define smart pointers for GIO data types

#include <glib.h>
#include <gio/gio.h>
#include "gobjectptr.h"
#include "cstrptr.h"

namespace Fm2 {

typedef GObjectPtr<GFile> GFilePtr;
typedef GObjectPtr<GFileInfo> GFileInfoPtr;
typedef GObjectPtr<GFileMonitor> GFileMonitorPtr;
typedef GObjectPtr<GCancellable> GCancellablePtr;

typedef GObjectPtr<GIcon> GIconPtr;

typedef GObjectPtr<GVolumeMonitor> GVolumeMonitorPtr;
typedef GObjectPtr<GVolume> GVolumePtr;
typedef GObjectPtr<GMount> GMountPtr;


class GErrorPtr {
public:
    GErrorPtr(): err_{nullptr} {
    }

    GErrorPtr(GError*&& err): err_{err} {
        err = nullptr;
    }

    GErrorPtr(const GErrorPtr& other) = delete;

    GErrorPtr(GErrorPtr&& other): err_{other.err_} {
        other.err_ = nullptr;
    }

    ~GErrorPtr() {
        reset();
    }

    void reset() {
        if(err_)
            g_error_free(err_);
    }

    GError* get() const {
        return err_;
    }

    GErrorPtr& operator = (const GErrorPtr& other) = delete;

    GErrorPtr& operator = (GErrorPtr&& other) {
        reset();
        err_ = other.err_;
        other.err_ = nullptr;
        return *this;
    }

    GErrorPtr& operator = (GError*&& err) {
        reset();
        err_ = err;
        err_ = nullptr;
        return *this;
    }

    GError** operator&() {
        return &err_;
    }

    GError* operator->() {
        return err_;
    }

    bool operator == (const GErrorPtr& other) const {
        return err_ == other.err_;
    }

    bool operator == (GError* err) const {
        return err_ == err;
    }

    bool operator != (std::nullptr_t) const {
        return err_ != nullptr;
    }

    operator bool() const {
        return err_ != nullptr;
    }

private:
    GError* err_;
};

} //namespace Fm2

#endif // GIOPTRS_H