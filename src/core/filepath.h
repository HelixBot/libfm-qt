#ifndef FM2_FILEPATH_H
#define FM2_FILEPATH_H

#include "gobjectptr.h"
#include "cstrptr.h"
#include <gio/gio.h>
#include <vector>


namespace Fm2 {

class FilePath {
public:

    explicit FilePath() {
    }

    explicit FilePath(const char *path_str): gfile_{g_file_new_for_commandline_arg(path_str), false} {
    }

    explicit FilePath(GFile* gfile, bool add_ref): gfile_{gfile, add_ref} {
    }

    FilePath(const FilePath& other): FilePath{} {
        *this = other;
    }

    FilePath(FilePath&& other): FilePath{} {
        *this = other;
    }

    static FilePath fromUri(const char* uri) {
        return FilePath{g_file_new_for_uri(uri), false};
    }

    static FilePath fromLocalPath(const char* path) {
        return FilePath{g_file_new_for_path(path), false};
    }

    bool isValid() const {
        return gfile_ != nullptr;
    }

    unsigned int hash() const {
        return g_file_hash(gfile_.get());
    }

    CStrPtr baseName() const {
        return CStrPtr{g_file_get_basename(gfile_.get())};
    }

    CStrPtr localPath() const {
        return CStrPtr{g_file_get_path(gfile_.get())};
    }

    CStrPtr uri() const {
        return CStrPtr{g_file_get_uri(gfile_.get())};
    }

    CStrPtr toString() const {
        if(isNative()) {
            return localPath();
        }
        return uri();
    }

    FilePath parent() const {
        return FilePath{g_file_get_parent(gfile_.get()), false};
    }

    bool isParentOf(const FilePath& other) {
        return g_file_has_parent(other.gfile_.get(), gfile_.get());
    }

    bool isPrefixOf(const FilePath& other) {
        return g_file_has_prefix(other.gfile_.get(), gfile_.get());
    }

    FilePath child(const char* name) const {
        return FilePath{g_file_get_child(gfile_.get(), name), false};
    }

    CStrPtr relativePathStr(const FilePath& descendant) const {
        return CStrPtr{g_file_get_relative_path(gfile_.get(), descendant.gfile_.get())};
    }

    FilePath relativePath(const char* relPath) const {
        return FilePath{g_file_resolve_relative_path(gfile_.get(), relPath), false};
    }

    bool isNative() const {
        return g_file_is_native(gfile_.get());
    }

    bool hasUriScheme(const char* scheme) const {
        return g_file_has_uri_scheme(gfile_.get(), scheme);
    }

    CStrPtr uriScheme() const {
        return CStrPtr{g_file_get_uri_scheme(gfile_.get())};
    }

    const GObjectPtr<GFile>& gfile() const {
        return gfile_;
    }

    FilePath& operator = (const FilePath& other) {
        gfile_ = other.gfile_;
        return *this;
    }

    FilePath& operator = (const FilePath&& other) {
        gfile_ = std::move(other.gfile_);
        return *this;
    }

    bool operator == (const FilePath& other) const {
        if(gfile_ == other.gfile_) {
            return true;
        }
        if(gfile_ && other.gfile_) {
            return g_file_equal(gfile_.get(), other.gfile_.get());
        }
        return false;
    }

private:
    GObjectPtr<GFile> gfile_;
};

struct FilePathHash {
    std::size_t operator() (const FilePath& path) const {
        return path.hash();
    }
};

typedef std::vector<FilePath> FilePathList;

} // namespace Fm2

#endif // FM2_FILEPATH_H