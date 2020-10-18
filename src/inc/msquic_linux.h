/*++

    Copyright (c) Microsoft Corporation.
    Licensed under the MIT License.

Abstract:

    This file contains the platform specific definitions for MsQuic structures
    and error codes.

Environment:

    Linux

--*/

#pragma once

#ifndef _MSQUIC_LINUX_
#define _MSQUIC_LINUX_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include "quic_sal_stub.h"

#ifdef __cplusplus
extern "C++" {
template <size_t S> struct _ENUM_FLAG_INTEGER_FOR_SIZE;
template <> struct _ENUM_FLAG_INTEGER_FOR_SIZE<1> {
    typedef uint8_t type;
};
template <> struct _ENUM_FLAG_INTEGER_FOR_SIZE<2> {
    typedef uint16_t type;
};
template <> struct _ENUM_FLAG_INTEGER_FOR_SIZE<4> {
    typedef uint32_t type;
};
template <> struct _ENUM_FLAG_INTEGER_FOR_SIZE<8> {
    typedef uint64_t type;
};

// used as an approximation of std::underlying_type<T>
template <class T> struct _ENUM_FLAG_SIZED_INTEGER
{
    typedef typename _ENUM_FLAG_INTEGER_FOR_SIZE<sizeof(T)>::type type;
};
}

#define DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE) \
extern "C++" { \
inline ENUMTYPE operator | (ENUMTYPE a, ENUMTYPE b) throw() { return ENUMTYPE(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) | ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
inline ENUMTYPE &operator |= (ENUMTYPE &a, ENUMTYPE b) throw() { return (ENUMTYPE &)(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) |= ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
inline ENUMTYPE operator & (ENUMTYPE a, ENUMTYPE b) throw() { return ENUMTYPE(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) & ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
inline ENUMTYPE &operator &= (ENUMTYPE &a, ENUMTYPE b) throw() { return (ENUMTYPE &)(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) &= ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
inline ENUMTYPE operator ~ (ENUMTYPE a) throw() { return ENUMTYPE(~((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a)); } \
inline ENUMTYPE operator ^ (ENUMTYPE a, ENUMTYPE b) throw() { return ENUMTYPE(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) ^ ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
inline ENUMTYPE &operator ^= (ENUMTYPE &a, ENUMTYPE b) throw() { return (ENUMTYPE &)(((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) ^= ((_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
}
#else
#define DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE) // NOP, C allows these operators.
#endif

#define QUIC_API
#define QUIC_MAIN_EXPORT
#define QUIC_STATUS                     unsigned int
#define QUIC_FAILED(X)                  ((int)(X) > 0)
#define QUIC_SUCCEEDED(X)               ((int)(X) <= 0)

//
// The type of an error code generated by the system is mostly 'int'.
// In many situations, we use the value of a system-generated error code
// as the value of QUIC_STATUS.
// In some situations, we use a custom value for QUIC_STATUS.
// In order to ensure that custom values don't conflict with system-generated values,
// the custom values are all kept outside the range of any possible 'int' value.
// There are static asserts to ensure that QUIC_STATUS type is large enough for
// this purpose. Below, all "ERROR_*" names represent custom error codes.
// All "E*" (that are not "ERROR_*") names are system error codes.
//

#define NO_ERROR                         0
#define ERROR_SUCCESS                    0
#define ERROR_CONTINUE                   -1
#define ERROR_NOT_READY                  -2
#define ERROR_BASE                       200000000
#define ERROR_NOT_ENOUGH_MEMORY          1 + ERROR_BASE
#define ERROR_INVALID_STATE              2 + ERROR_BASE
#define ERROR_INVALID_PARAMETER          3 + ERROR_BASE
#define ERROR_NOT_SUPPORTED              4 + ERROR_BASE
#define ERROR_NOT_FOUND                  5 + ERROR_BASE
#define ERROR_BUFFER_OVERFLOW            6 + ERROR_BASE
#define ERROR_CONNECTION_REFUSED         7 + ERROR_BASE
#define ERROR_OPERATION_ABORTED          8 + ERROR_BASE
#define ERROR_HANDSHAKE_FAILURE          9 + ERROR_BASE
#define ERROR_NETWORK_UNREACHABLE        10 + ERROR_BASE
#define ERROR_CONNECTION_IDLE            11 + ERROR_BASE
#define ERROR_INTERNAL_ERROR             12 + ERROR_BASE
#define ERROR_PROTOCOL_ERROR             13 + ERROR_BASE
#define ERROR_VER_NEG_ERROR              14 + ERROR_BASE
#define ERROR_EPOLL_ERROR                15 + ERROR_BASE
#define ERROR_DNS_RESOLUTION_ERROR       16 + ERROR_BASE
#define ERROR_SOCKET_ERROR               17 + ERROR_BASE
#define ERROR_SSL_ERROR                  18 + ERROR_BASE
#define ERROR_USER_CANCELED              19 + ERROR_BASE
#define ERROR_ALPN_NEG_FAILURE           20 + ERROR_BASE

#define QUIC_STATUS_SUCCESS             ((QUIC_STATUS)ERROR_SUCCESS)
#define QUIC_STATUS_PENDING             ((QUIC_STATUS)ERROR_NOT_READY)
#define QUIC_STATUS_CONTINUE            ((QUIC_STATUS)ERROR_CONTINUE)
#define QUIC_STATUS_OUT_OF_MEMORY       ((QUIC_STATUS)ENOMEM)
#define QUIC_STATUS_INVALID_PARAMETER   ((QUIC_STATUS)EINVAL)
#define QUIC_STATUS_INVALID_STATE       ((QUIC_STATUS)ERROR_INVALID_STATE)
#define QUIC_STATUS_NOT_SUPPORTED       ((QUIC_STATUS)EOPNOTSUPP)
#define QUIC_STATUS_NOT_FOUND           ((QUIC_STATUS)ENOENT)
#define QUIC_STATUS_BUFFER_TOO_SMALL    ((QUIC_STATUS)EOVERFLOW)
#define QUIC_STATUS_HANDSHAKE_FAILURE   ((QUIC_STATUS)ERROR_HANDSHAKE_FAILURE)
#define QUIC_STATUS_ABORTED             ((QUIC_STATUS)ERROR_OPERATION_ABORTED)
#define QUIC_STATUS_ADDRESS_IN_USE      ((QUIC_STATUS)EADDRINUSE)
#define QUIC_STATUS_CONNECTION_TIMEOUT  ((QUIC_STATUS)ETIMEDOUT)
#define QUIC_STATUS_CONNECTION_IDLE     ((QUIC_STATUS)ERROR_CONNECTION_IDLE)
#define QUIC_STATUS_INTERNAL_ERROR      ((QUIC_STATUS)ERROR_INTERNAL_ERROR)
#define QUIC_STATUS_CONNECTION_REFUSED  ((QUIC_STATUS)ERROR_CONNECTION_REFUSED)
#define QUIC_STATUS_PROTOCOL_ERROR      ((QUIC_STATUS)ERROR_PROTOCOL_ERROR)
#define QUIC_STATUS_VER_NEG_ERROR       ((QUIC_STATUS)ERROR_VER_NEG_ERROR)
#define QUIC_STATUS_UNREACHABLE         ((QUIC_STATUS)EHOSTUNREACH)
#define QUIC_STATUS_PERMISSION_DENIED   ((QUIC_STATUS)EPERM)
#define QUIC_STATUS_EPOLL_ERROR         ((QUIC_STATUS)ERROR_EPOLL_ERROR)
#define QUIC_STATUS_DNS_RESOLUTION_ERROR ((QUIC_STATUS)ERROR_DNS_RESOLUTION_ERROR)
#define QUIC_STATUS_SOCKET_ERROR        ((QUIC_STATUS)ERROR_SOCKET_ERROR)
#define QUIC_STATUS_TLS_ERROR           ((QUIC_STATUS)ERROR_SSL_ERROR)
#define QUIC_STATUS_USER_CANCELED       ((QUIC_STATUS)ERROR_USER_CANCELED)
#define QUIC_STATUS_ALPN_NEG_FAILURE    ((QUIC_STATUS)ERROR_ALPN_NEG_FAILURE)

typedef unsigned char BOOLEAN;
typedef struct in_addr IN_ADDR;
typedef struct in6_addr IN6_ADDR;
typedef struct addrinfo ADDRINFO;
typedef sa_family_t QUIC_ADDRESS_FAMILY;

//
// Defines match windows values.
//
#define QUIC_ADDRESS_FAMILY_UNSPEC AF_UNSPEC
#define QUIC_ADDRESS_FAMILY_INET AF_INET
#define QUIC_ADDRESS_FAMILY_INET6 AF_INET6

typedef union QUIC_ADDR {
    struct sockaddr Ip;
    struct sockaddr_in Ipv4;
    struct sockaddr_in6 Ipv6;
} QUIC_ADDR;

#define FIELD_OFFSET(type, field)       offsetof(type, field)

#define QUIC_ADDR_V4_PORT_OFFSET        FIELD_OFFSET(struct sockaddr_in, sin_port)
#define QUIC_ADDR_V4_IP_OFFSET          FIELD_OFFSET(struct sockaddr_in, sin_addr)

#define QUIC_ADDR_V6_PORT_OFFSET        FIELD_OFFSET(struct sockaddr_in6, sin6_port)
#define QUIC_ADDR_V6_IP_OFFSET          FIELD_OFFSET(struct sockaddr_in6, sin6_addr)

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

#define INITCODE
#define PAGEDX
#define QUIC_CACHEALIGN

#if defined(__cplusplus)
extern "C" {
#endif

#define QUIC_LOCALHOST_FOR_AF(Af) ((Af == QUIC_ADDRESS_FAMILY_INET) ? "localhost" : "ip6-localhost")

#define QUIC_CERTIFICATE_FLAG_IGNORE_REVOCATION                 0x00000080
#define QUIC_CERTIFICATE_FLAG_IGNORE_UNKNOWN_CA                 0x00000100
#define QUIC_CERTIFICATE_FLAG_IGNORE_WRONG_USAGE                0x00000200
#define QUIC_CERTIFICATE_FLAG_IGNORE_CERTIFICATE_CN_INVALID     0x00001000 // bad common name in X509 Cert.
#define QUIC_CERTIFICATE_FLAG_IGNORE_CERTIFICATE_DATE_INVALID   0x00002000 // expired X509 Cert.
#define QUIC_CERTIFICATE_FLAG_IGNORE_WEAK_SIGNATURE             0x00010000

#if defined(__clang__)
#define QUIC_NO_SANITIZE(X) __attribute__((no_sanitize(X)))
#else
#define QUIC_NO_SANITIZE(X)
#endif

//
// Helpers for Windows string functions.
//

#define _strnicmp strncasecmp
#define sprintf_s(dst, dst_len, format, ...) sprintf(dst, format, __VA_ARGS__)
#define _vsnprintf_s(dst, dst_len, flag, format, ...) vsnprintf(dst, dst_len, format, __VA_ARGS__)

//
// Hack to pass in a client trusted cert from APP layer to the TAL layer.
//

extern char *QuicOpenSslClientTrustedCert;

//
// IP Address Abstraction Helpers
//

inline
BOOLEAN
QuicAddrFamilyIsValid(
    _In_ QUIC_ADDRESS_FAMILY Family
    )
{
    return
        Family == QUIC_ADDRESS_FAMILY_UNSPEC ||
        Family == QUIC_ADDRESS_FAMILY_INET ||
        Family == QUIC_ADDRESS_FAMILY_INET6;
}

inline
BOOLEAN
QuicAddrIsValid(
    _In_ const QUIC_ADDR* const Addr
    )
{
    return QuicAddrFamilyIsValid(Addr->Ip.sa_family);
}

inline
BOOLEAN
QuicAddrCompareIp(
    _In_ const QUIC_ADDR* const Addr1,
    _In_ const QUIC_ADDR* const Addr2
    )
{
    if (QUIC_ADDRESS_FAMILY_INET == Addr1->Ip.sa_family) {
        return memcmp(&Addr1->Ipv4.sin_addr, &Addr2->Ipv4.sin_addr, sizeof(IN_ADDR)) == 0;
    } else {
        return memcmp(&Addr1->Ipv6.sin6_addr, &Addr2->Ipv6.sin6_addr, sizeof(IN6_ADDR)) == 0;
    }
}

inline
BOOLEAN
QuicAddrCompare(
    _In_ const QUIC_ADDR* const Addr1,
    _In_ const QUIC_ADDR* const Addr2
    )
{
    if (Addr1->Ip.sa_family != Addr2->Ip.sa_family ||
        Addr1->Ipv4.sin_port != Addr2->Ipv4.sin_port) {
        return FALSE;
    }

    if (QUIC_ADDRESS_FAMILY_INET == Addr1->Ip.sa_family) {
        return memcmp(&Addr1->Ipv4.sin_addr, &Addr2->Ipv4.sin_addr, sizeof(IN_ADDR)) == 0;
    } else {
        return memcmp(&Addr1->Ipv6.sin6_addr, &Addr2->Ipv6.sin6_addr, sizeof(IN6_ADDR)) == 0;
    }
}

inline
QUIC_ADDRESS_FAMILY
QuicAddrGetFamily(
    _In_ const QUIC_ADDR* const Addr
    )
{
    return Addr->Ip.sa_family;
}

inline
void
QuicAddrSetFamily(
    _In_ QUIC_ADDR* Addr,
    _In_ QUIC_ADDRESS_FAMILY Family
    )
{
    Addr->Ip.sa_family = Family;
}

inline
uint16_t
QuicAddrGetPort(
    _In_ const QUIC_ADDR* const Addr
    )
{
    if (QUIC_ADDRESS_FAMILY_INET == Addr->Ip.sa_family) {
        return ntohs(Addr->Ipv4.sin_port);
    } else {
        return ntohs(Addr->Ipv6.sin6_port);
    }
}

inline
void
QuicAddrSetPort(
    _Out_ QUIC_ADDR* Addr,
    _In_ uint16_t Port
    )
{
    if (QUIC_ADDRESS_FAMILY_INET == Addr->Ip.sa_family) {
        Addr->Ipv4.sin_port = htons(Port);
    } else {
        Addr->Ipv6.sin6_port = htons(Port);
    }
}

//
// Test only API to increment the IP address value.
//
inline
void
QuicAddrIncrement(
    _Inout_ QUIC_ADDR* Addr
    )
{
    if (Addr->Ip.sa_family == QUIC_ADDRESS_FAMILY_INET) {
        ((uint8_t*)&Addr->Ipv4.sin_addr)[3]++;
    } else {
        ((uint8_t*)&Addr->Ipv6.sin6_addr)[15]++;
    }
}

inline
void
QuicAddrSetToLoopback(
    _Inout_ QUIC_ADDR* Addr
    )
{
    if (Addr->Ip.sa_family == QUIC_ADDRESS_FAMILY_INET) {
        Addr->Ipv4.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    } else {
        Addr->Ipv6.sin6_addr = in6addr_loopback;
    }
}

inline
uint32_t
QUIC_NO_SANITIZE("unsigned-integer-overflow")
QuicAddrHash(
    _In_ const QUIC_ADDR* Addr
    )
{
    uint32_t Hash = 5387; // A random prime number.
#define UPDATE_HASH(byte) Hash = ((Hash << 5) - Hash) + (byte)
    if (Addr->Ip.sa_family == QUIC_ADDRESS_FAMILY_INET) {
        UPDATE_HASH(Addr->Ipv4.sin_port & 0xFF);
        UPDATE_HASH(Addr->Ipv4.sin_port >> 8);
        for (uint8_t i = 0; i < sizeof(Addr->Ipv4.sin_addr); ++i) {
            UPDATE_HASH(((uint8_t*)&Addr->Ipv4.sin_addr)[i]);
        }
    } else {
        UPDATE_HASH(Addr->Ipv6.sin6_port & 0xFF);
        UPDATE_HASH(Addr->Ipv6.sin6_port >> 8);
        for (uint8_t i = 0; i < sizeof(Addr->Ipv6.sin6_addr); ++i) {
            UPDATE_HASH(((uint8_t*)&Addr->Ipv6.sin6_addr)[i]);
        }
    }
    return Hash;
}

inline
BOOLEAN
QuicAddrIsWildCard(
    _In_ const QUIC_ADDR* const Addr
    )
{
    if (Addr->Ip.sa_family == QUIC_ADDRESS_FAMILY_UNSPEC) {
        return TRUE;
    } else if (Addr->Ip.sa_family == QUIC_ADDRESS_FAMILY_INET) {
        const IN_ADDR ZeroAddr = {0};
        return memcmp(&Addr->Ipv4.sin_addr.s_addr, &ZeroAddr, sizeof(IN_ADDR)) == 0;
    } else {
        const IN6_ADDR ZeroAddr = {0};
        return memcmp(&Addr->Ipv6.sin6_addr, &ZeroAddr, sizeof(IN6_ADDR)) == 0;
    }
}

inline
void
QuicConvertToMappedV6(
    _In_ const QUIC_ADDR* InAddr,
    _Out_ QUIC_ADDR* OutAddr
    )
{
    //QUIC_DBG_ASSERT(!(InAddr == OutAddr));
    if (InAddr->Ip.sa_family == QUIC_ADDRESS_FAMILY_INET) {
        memset(OutAddr, 0, sizeof(QUIC_ADDR));
        OutAddr->Ipv6.sin6_family = QUIC_ADDRESS_FAMILY_INET6;
        OutAddr->Ipv6.sin6_port = InAddr->Ipv4.sin_port;
        memset(&(OutAddr->Ipv6.sin6_addr.s6_addr[10]), 0xff, 2);
        memcpy(&(OutAddr->Ipv6.sin6_addr.s6_addr[12]), &InAddr->Ipv4.sin_addr.s_addr, 4);
    } else {
        *OutAddr = *InAddr;
    }
}

inline
void
QuicConvertFromMappedV6(
    _In_ const QUIC_ADDR* InAddr,
    _Out_ QUIC_ADDR* OutAddr
    )
{
    //QUIC_DBG_ASSERT(InAddr->Ip.sa_family == QUIC_ADDRESS_FAMILY_INET6);
    if (IN6_IS_ADDR_V4MAPPED(&InAddr->Ipv6.sin6_addr)) {
        QUIC_ADDR TmpAddrS = {0};
        QUIC_ADDR* TmpAddr = &TmpAddrS;
        TmpAddr->Ipv4.sin_family = QUIC_ADDRESS_FAMILY_INET;
        TmpAddr->Ipv4.sin_port = InAddr->Ipv6.sin6_port;
        memcpy(&TmpAddr->Ipv4.sin_addr.s_addr, &InAddr->Ipv6.sin6_addr.s6_addr[12], 4);
        *OutAddr = *TmpAddr;
    } else if (OutAddr != InAddr) {
        *OutAddr = *InAddr;
    }
}

inline
BOOLEAN
QuicAddr4FromString(
    _In_z_ const char* AddrStr,
    _Out_ QUIC_ADDR* Addr
    )
{
    if (AddrStr[0] == '[') {
        return FALSE;
    }

    const char* PortStart = strchr(AddrStr, ':');
    if (PortStart != NULL) {
        if (strchr(PortStart+1, ':') != NULL) {
            return FALSE;
        }

        char TmpAddrStr[16];
        size_t AddrLength = PortStart - AddrStr;
        if (AddrLength >= sizeof(TmpAddrStr)) {
            return FALSE;
        }
        memcpy(TmpAddrStr, AddrStr, AddrLength);
        TmpAddrStr[AddrLength] = '\0';

        if (inet_pton(AF_INET, TmpAddrStr, &Addr->Ipv4.sin_addr) != 1) {
            return FALSE;
        }
        Addr->Ipv4.sin_port = htons(atoi(PortStart+1));
    } else {
        if (inet_pton(AF_INET, AddrStr, &Addr->Ipv4.sin_addr) != 1) {
            return FALSE;
        }
    }
    Addr->Ip.sa_family = QUIC_ADDRESS_FAMILY_INET;
    return TRUE;
}

inline
BOOLEAN
QuicAddr6FromString(
    _In_z_ const char* AddrStr,
    _Out_ QUIC_ADDR* Addr
    )
{
    if (AddrStr[0] == '[') {
        const char* BracketEnd = strchr(AddrStr, ']');
        if (BracketEnd == NULL || *(BracketEnd+1) != ':') {
            return FALSE;
        }

        char TmpAddrStr[64];
        size_t AddrLength = BracketEnd - AddrStr;
        if (AddrLength >= sizeof(TmpAddrStr)) {
            return FALSE;
        }
        memcpy(TmpAddrStr, AddrStr, AddrLength);
        TmpAddrStr[AddrLength] = '\0';

        if (inet_pton(AF_INET6, TmpAddrStr, &Addr->Ipv6.sin6_addr) != 1) {
            return FALSE;
        }
        Addr->Ipv6.sin6_port = htons(atoi(BracketEnd+2));
    } else {
        if (inet_pton(AF_INET6, AddrStr, &Addr->Ipv6.sin6_addr) != 1) {
            return FALSE;
        }
    }
    Addr->Ip.sa_family = QUIC_ADDRESS_FAMILY_INET6;
    return TRUE;
}

inline
BOOLEAN
QuicAddrFromString(
    _In_z_ const char* AddrStr,
    _In_ uint16_t Port, // Host byte order
    _Out_ QUIC_ADDR* Addr
    )
{
    Addr->Ipv4.sin_port = htons(Port);
    return
        QuicAddr4FromString(AddrStr, Addr) ||
        QuicAddr6FromString(AddrStr, Addr);
}

//
// Represents an IP address and (optionally) port number as a string.
//
typedef struct QUIC_ADDR_STR {
    char Address[64];
} QUIC_ADDR_STR;

inline
BOOLEAN
QuicAddrToString(
    _In_ const QUIC_ADDR* Addr,
    _Out_ QUIC_ADDR_STR* AddrStr
    )
{
    char* Address = AddrStr->Address;
    if (Addr->Ip.sa_family == QUIC_ADDRESS_FAMILY_INET6 && Addr->Ipv6.sin6_port != 0) {
        Address[0] = '[';
        Address++;
    }
    if (inet_ntop(
            Addr->Ip.sa_family,
            &Addr->Ipv4.sin_addr,
            Address,
            sizeof(QUIC_ADDR_STR)) == NULL) {
        return FALSE;
    }
    if (Addr->Ipv4.sin_port != 0) {
        Address += strlen(Address);
        if (Addr->Ip.sa_family == QUIC_ADDRESS_FAMILY_INET6) {
            Address[0] = ']';
            Address++;
        }
        sprintf(Address, ":%hu", ntohs(Addr->Ipv4.sin_port));
    }
    return TRUE;
}

#if defined(__cplusplus)
}
#endif

#endif
