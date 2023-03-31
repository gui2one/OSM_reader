#ifndef UTILS_H
#define UTILS_H
#pragma once

namespace Utils{
    

    static bool str_is_equal(const char* str1, const char* str2)
    {
        return strcmp(str1, str2) == 0;
    }
}

#endif /* UTILS_H */