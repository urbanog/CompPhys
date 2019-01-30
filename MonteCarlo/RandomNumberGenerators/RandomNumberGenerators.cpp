//------------------------------------------------------------------------------
/// \file RandomNumberGenerators.cpp
/// \author Ernest Yeung
/// \email  ernestyalumni@gmail.com
/// \brief  Random number generators (RNG).
/// \url https://github.com/CompPhysics/ComputationalPhysics/blob/master/doc/Programs/LecturePrograms/programs/MCIntro/cpp/lib.cpp
/// \ref Ch. 21 Class Hierarchies, 21.2.Design of Class Hierarchies
///   The C++ Programming Language, 4th Ed., Stroustrup;
/// \details Random number generators..
/// \copyright If you find this code useful, feel free to donate directly
/// (username ernestyalumni or email address above), going directly to:
///
/// paypal.me/ernestyalumni
///
/// which won't go through a 3rd. party like indiegogo, kickstarter, patreon.
/// Otherwise, I receive emails and messages on how all my (free) material on
/// physics, math, and engineering have helped students with their studies, and
/// I know what it's like to not have money as a student, but love physics (or
/// math, sciences, etc.), so I am committed to keeping all my material
/// open-source and free, whether or not sufficiently crowdfunded, under the
/// open-source MIT license: feel free to copy, edit, paste, make your own
/// versions, share, use as you wish.
/// Peace out, never give up! -EY
//------------------------------------------------------------------------------
/// COMPILATION TIPS:
///  g++ -std=c++17 RandomNumberGenerators.cpp RandomNumberGenerators_main.cpp \
///   -o RandomNumberGenerators_main
//------------------------------------------------------------------------------
#include "RandomNumberGenerators.h"

namespace MonteCarlo
{

namespace RandomNumberGenerators
{

MinimalParkMiller::MinimalParkMiller()//:
//  idum{nullptr}
{}

MinimalParkMiller::MinimalParkMiller(const long seed)//:
//  idum{nullptr}
{}

double MinimalParkMiller::operator()(long* idum)
//double MinimalParkMiller::operator()()
{
  *idum ^= MASK_;
  long k {(*idum) / IQ_};
  *idum = IA_ * (*idum - k * IQ_) - IR_ * k;

  if (*idum < 0)
  {
    *idum += IM_;
  }

  double ans {AM_ * (*idum)};
  *idum ^= MASK_;

  return ans;
}

BaysDurhamShuffle::BaysDurhamShuffle()
{}

double BaysDurhamShuffle::operator()(long* idum)
//double BaysDurhamShuffle::operator()()
{
  int j;
  long k;

  static long iy {0};
  static long iv[NTAB_];

  double temp;

  if (*idum <= 0 || !iy)
  {
    (-(*idum) < 1) ? *idum = 1 : *idum = -(*idum);

    for (j = NTAB_ + 7; j >= 0; j--)
    {
      k = (*idum) / IQ_;
      *idum = IA_ * (*idum - k * IQ_) - IR_ * k;
      if (*idum < 0)
      {
        *idum += IM_;
      }

      if (j < NTAB_)
      {
        iv[j] = *idum;
      }
    }
    iy = iv[0];
  }

  k = (*idum) / IQ_;
  *idum = IA_ * (*idum - k * IQ_) - IR_ * k;

  if (*idum < 0)
  {
    *idum += IM_;
  }

  j = iy / NDIV_;
  iy = iv[j];
  iv[j] = *idum;

  return ((AM_* iy) > RNMX_) ? RNMX_ : AM_ * iy;
}

LEcuyer::LEcuyer()
{}

double LEcuyer::operator()(long* idum)
{
  int j;
  long k;
  static long idum2 {123456789};
  static long iv[NTAB_];

  static long iy {0};

  if (*idum <= 0)
  {
    (-(*idum) < 1) ? *idum = 1 : *idum = (-(*idum));

    idum2 = (*idum);

    for (j = NTAB_ + 7; j >= 0; j--)
    {
      k = (*idum) / IQ1_;
      *idum = IA1_ * (*idum - k * IQ1_) - k * IR1_;

      if (*idum < 0)
      {
        *idum += IM1_;
      }

      if (j < NTAB_)
      {
        iv[j] = *idum;
      }
    }
    iy = iv[0];
  }

  k = (*idum) / IQ1_;

  *idum = IA1_ * (*idum - k * IQ1_) - k * IR1_;

  if (*idum < 0)
  {
    *idum += IM1_;
  }

  k = idum2 / IQ2_;

  idum2 = IA2_ * (idum2 - k * IQ2_) - k * IR2_;

  if (idum2 < 0)
  {
    idum2 += IM2_;
  }

  j = iy / NDIV_;

  iy = iv[j] - idum2;

  iv[j] = *idum;

  if (iy < 1)
  {
    iy += IMM1_;
  }

  return ((AM1_ * iy) > RNMX_) ? RNMX_ : AM1_ * iy;
}

Uniform::Uniform()
{}

double Uniform::operator()(long *idum)
//double Uniform::operator()()
{
  static int inext, inextp;
  static long ma[56]; // value 56 is special, do not modify
  static int iff {0};

  long mj;

  if (*idum < 0 || iff == 0) // initialization
  {
    iff = 1;

    mj = MSEED_ - (*idum < 0 ? -*idum : *idum);
    mj %= MBIG_;
    ma[55] = mj; // initialize ma[55]

    for (int i {1}, mk {1}; i <= 54; i++) // initialize rest of table
    {
      int ii {(21 * i) % 55};
      ma[ii] = mk;
      mk = mj - mk;
      if (mk < MZ_)
      {
        mk += MBIG_;
      }
      mj = ma[ii];
    }

    for (int k {0}; k < 4; ++k)
    {
      // randomize by "warming up" the generator
      for (int i {1}; i <= 55; ++i)
      {
        ma[i] -= ma[1 + (i + 30) % 55];
        if (ma[i] < MZ_)
        {
          ma[i] += MBIG_;
        }
      }
    }

    inext = 0; // prepare indices for first generator number
    inextp = 31; // 31 is special
    *idum = 1;
  }

  if (++inext == 56)
  {
    inext = 1;
  }

  if (++inextp == 56)
  {
    inextp = 1;
  }

  mj = ma[inext] - ma[inextp];

  if (mj < MZ_)
  {
    mj += MBIG_;
  }

  ma[inext] = mj;

  return mj * FAC_;
}

} // namespace RandomNumberGenerators

} // namespace MonteCarlo