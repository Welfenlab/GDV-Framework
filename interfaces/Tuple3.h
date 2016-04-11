#ifndef TUPLE3_H
#define TUPLE3_H

#include <QtGlobal>

/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


/**
 * @brief Die Tuple3 Klasse
 *
 * Diese Klasse erlaubt es, drei Instanzen einer beliebigen Klasse bzw. eines
 * beliebigen Structs zusammenzufassen. Da alle wichtigen Copy-Konstruktoren
 * implementiert sind, eignet sich die Klasse auch für verschachtelte
 * Konstrukte.
 *
 * Beispiel zur Verwendung:
 *
 * Tuple3<float> foo;
 * for(int n = 0; n < 3; n++)
 *   foo[n] = 42.0f * n;
 *
 * Tuple3<float> bar = foo;
 *
 * TODO: Doku -> Foreach nicht möglich!
 *
 */
template<class T>
class Tuple3
{
public:
    Tuple3() { }

    Tuple3(const Tuple3<T>& other)
    {
        for(int n = 0; n < 3; n++)
            values[n] = other[n];
    }

    Tuple3<T>& operator=(const Tuple3<T>& other)
    {
        for(int n = 0; n < 3; n++)
            values[n] = other[n];
        return *this;
    }

    inline T& operator[](int p)
    {
        Q_ASSERT(p >= 0 && p < 3);
        return values[p];
    }

    inline const T& operator[](int p) const
    {
        Q_ASSERT(p >= 0 && p < 3);
        return values[p];
    }


private:
    T values[3];
};

#endif // TUPLE3_H
