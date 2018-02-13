// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Message.proto

#ifndef PROTOBUF_Message_2eproto__INCLUDED
#define PROTOBUF_Message_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace google {
namespace protobuf {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_Message_2eproto();
void protobuf_AssignDesc_Message_2eproto();
void protobuf_ShutdownFile_Message_2eproto();

class HN_RET_LOGIN;
class NH_REQ_LOGIN;

// ===================================================================

class NH_REQ_LOGIN : public ::google::protobuf::Message {
 public:
  NH_REQ_LOGIN();
  virtual ~NH_REQ_LOGIN();

  NH_REQ_LOGIN(const NH_REQ_LOGIN& from);

  inline NH_REQ_LOGIN& operator=(const NH_REQ_LOGIN& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const NH_REQ_LOGIN& default_instance();

  void Swap(NH_REQ_LOGIN* other);

  // implements Message ----------------------------------------------

  inline NH_REQ_LOGIN* New() const { return New(NULL); }

  NH_REQ_LOGIN* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const NH_REQ_LOGIN& from);
  void MergeFrom(const NH_REQ_LOGIN& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(NH_REQ_LOGIN* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string account = 1;
  void clear_account();
  static const int kAccountFieldNumber = 1;
  const ::std::string& account() const;
  void set_account(const ::std::string& value);
  void set_account(const char* value);
  void set_account(const char* value, size_t size);
  ::std::string* mutable_account();
  ::std::string* release_account();
  void set_allocated_account(::std::string* account);

  // optional string validateinfo = 2;
  void clear_validateinfo();
  static const int kValidateinfoFieldNumber = 2;
  const ::std::string& validateinfo() const;
  void set_validateinfo(const ::std::string& value);
  void set_validateinfo(const char* value);
  void set_validateinfo(const char* value, size_t size);
  ::std::string* mutable_validateinfo();
  ::std::string* release_validateinfo();
  void set_allocated_validateinfo(::std::string* validateinfo);

  // @@protoc_insertion_point(class_scope:google.protobuf.NH_REQ_LOGIN)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr account_;
  ::google::protobuf::internal::ArenaStringPtr validateinfo_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Message_2eproto();
  friend void protobuf_AssignDesc_Message_2eproto();
  friend void protobuf_ShutdownFile_Message_2eproto();

  void InitAsDefaultInstance();
  static NH_REQ_LOGIN* default_instance_;
};
// -------------------------------------------------------------------

class HN_RET_LOGIN : public ::google::protobuf::Message {
 public:
  HN_RET_LOGIN();
  virtual ~HN_RET_LOGIN();

  HN_RET_LOGIN(const HN_RET_LOGIN& from);

  inline HN_RET_LOGIN& operator=(const HN_RET_LOGIN& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const HN_RET_LOGIN& default_instance();

  void Swap(HN_RET_LOGIN* other);

  // implements Message ----------------------------------------------

  inline HN_RET_LOGIN* New() const { return New(NULL); }

  HN_RET_LOGIN* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const HN_RET_LOGIN& from);
  void MergeFrom(const HN_RET_LOGIN& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(HN_RET_LOGIN* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string account = 1;
  void clear_account();
  static const int kAccountFieldNumber = 1;
  const ::std::string& account() const;
  void set_account(const ::std::string& value);
  void set_account(const char* value);
  void set_account(const char* value, size_t size);
  ::std::string* mutable_account();
  ::std::string* release_account();
  void set_allocated_account(::std::string* account);

  // optional int32 result = 2;
  void clear_result();
  static const int kResultFieldNumber = 2;
  ::google::protobuf::int32 result() const;
  void set_result(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:google.protobuf.HN_RET_LOGIN)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr account_;
  ::google::protobuf::int32 result_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_Message_2eproto();
  friend void protobuf_AssignDesc_Message_2eproto();
  friend void protobuf_ShutdownFile_Message_2eproto();

  void InitAsDefaultInstance();
  static HN_RET_LOGIN* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// NH_REQ_LOGIN

// optional string account = 1;
inline void NH_REQ_LOGIN::clear_account() {
  account_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& NH_REQ_LOGIN::account() const {
  // @@protoc_insertion_point(field_get:google.protobuf.NH_REQ_LOGIN.account)
  return account_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NH_REQ_LOGIN::set_account(const ::std::string& value) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:google.protobuf.NH_REQ_LOGIN.account)
}
inline void NH_REQ_LOGIN::set_account(const char* value) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:google.protobuf.NH_REQ_LOGIN.account)
}
inline void NH_REQ_LOGIN::set_account(const char* value, size_t size) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:google.protobuf.NH_REQ_LOGIN.account)
}
inline ::std::string* NH_REQ_LOGIN::mutable_account() {
  
  // @@protoc_insertion_point(field_mutable:google.protobuf.NH_REQ_LOGIN.account)
  return account_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* NH_REQ_LOGIN::release_account() {
  
  return account_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NH_REQ_LOGIN::set_allocated_account(::std::string* account) {
  if (account != NULL) {
    
  } else {
    
  }
  account_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), account);
  // @@protoc_insertion_point(field_set_allocated:google.protobuf.NH_REQ_LOGIN.account)
}

// optional string validateinfo = 2;
inline void NH_REQ_LOGIN::clear_validateinfo() {
  validateinfo_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& NH_REQ_LOGIN::validateinfo() const {
  // @@protoc_insertion_point(field_get:google.protobuf.NH_REQ_LOGIN.validateinfo)
  return validateinfo_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NH_REQ_LOGIN::set_validateinfo(const ::std::string& value) {
  
  validateinfo_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:google.protobuf.NH_REQ_LOGIN.validateinfo)
}
inline void NH_REQ_LOGIN::set_validateinfo(const char* value) {
  
  validateinfo_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:google.protobuf.NH_REQ_LOGIN.validateinfo)
}
inline void NH_REQ_LOGIN::set_validateinfo(const char* value, size_t size) {
  
  validateinfo_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:google.protobuf.NH_REQ_LOGIN.validateinfo)
}
inline ::std::string* NH_REQ_LOGIN::mutable_validateinfo() {
  
  // @@protoc_insertion_point(field_mutable:google.protobuf.NH_REQ_LOGIN.validateinfo)
  return validateinfo_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* NH_REQ_LOGIN::release_validateinfo() {
  
  return validateinfo_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NH_REQ_LOGIN::set_allocated_validateinfo(::std::string* validateinfo) {
  if (validateinfo != NULL) {
    
  } else {
    
  }
  validateinfo_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), validateinfo);
  // @@protoc_insertion_point(field_set_allocated:google.protobuf.NH_REQ_LOGIN.validateinfo)
}

// -------------------------------------------------------------------

// HN_RET_LOGIN

// optional string account = 1;
inline void HN_RET_LOGIN::clear_account() {
  account_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& HN_RET_LOGIN::account() const {
  // @@protoc_insertion_point(field_get:google.protobuf.HN_RET_LOGIN.account)
  return account_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void HN_RET_LOGIN::set_account(const ::std::string& value) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:google.protobuf.HN_RET_LOGIN.account)
}
inline void HN_RET_LOGIN::set_account(const char* value) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:google.protobuf.HN_RET_LOGIN.account)
}
inline void HN_RET_LOGIN::set_account(const char* value, size_t size) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:google.protobuf.HN_RET_LOGIN.account)
}
inline ::std::string* HN_RET_LOGIN::mutable_account() {
  
  // @@protoc_insertion_point(field_mutable:google.protobuf.HN_RET_LOGIN.account)
  return account_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* HN_RET_LOGIN::release_account() {
  
  return account_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void HN_RET_LOGIN::set_allocated_account(::std::string* account) {
  if (account != NULL) {
    
  } else {
    
  }
  account_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), account);
  // @@protoc_insertion_point(field_set_allocated:google.protobuf.HN_RET_LOGIN.account)
}

// optional int32 result = 2;
inline void HN_RET_LOGIN::clear_result() {
  result_ = 0;
}
inline ::google::protobuf::int32 HN_RET_LOGIN::result() const {
  // @@protoc_insertion_point(field_get:google.protobuf.HN_RET_LOGIN.result)
  return result_;
}
inline void HN_RET_LOGIN::set_result(::google::protobuf::int32 value) {
  
  result_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.HN_RET_LOGIN.result)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Message_2eproto__INCLUDED
