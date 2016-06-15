/****************************************************************************
* Italian National Research Council                                         *
* Institute for Applied Mathematics and Information Technologies, Genoa     *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Author: Marco Livesu (marco.livesu@gmail.com)                             *
*                                                                           *
* Copyright(C) 2016                                                         *
* All rights reserved.                                                      *
*                                                                           *
* This file is part of CinoLib                                              *
*                                                                           *
* CinoLib is free software; you can redistribute it and/or modify           *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
****************************************************************************/
#include "ssvd.h"
#include "common.h"

#include <iostream>

namespace cinolib
{

CINO_INLINE
void ssvd(const Eigen::Matrix3d                 & m,
                Eigen::Matrix3d                 & u,
                Eigen::DiagonalMatrix<double,3> & s,
                Eigen::Matrix3d                 & v)
{
    Eigen::JacobiSVD<Matrix3d> svd(m, ComputeFullU | ComputeFullV);

    s = Eigen::DiagonalMatrix<double,3>(svd.singularValues());
    u = svd.matrixU();
    v = svd.matrixV();

    if (v.determinant() < 0)
    {
        v.coeffRef(0,2) = -v.coeff(0,2);
        v.coeffRef(1,2) = -v.coeff(1,2);
        v.coeffRef(2,2) = -v.coeff(2,2);

        u.coeffRef(0,2) = - u.coeff(0,2);
        u.coeffRef(1,2) = - u.coeff(1,2);
        u.coeffRef(2,2) = - u.coeff(2,2);
    }

    assert(v.determinant() > 0);

    if (u.determinant() < 0)
    {
        u.coeffRef(0,2) = -u.coeff(0,2);
        u.coeffRef(1,2) = -u.coeff(1,2);
        u.coeffRef(2,2) = -u.coeff(2,2);

        s.diagonal()[2] = -s.diagonal()[2];
    }

    assert(u.determinant() > 0);
    assert(m.determinant() * (u * s * v.transpose()).determinant() > 0);

    //cout << "m :\n" << m << endl;
    //cout << endl;
    //cout << "m':\n" << u * s * v.transpose() << endl;
    //cout << endl;
    //cout << "m' - m:\n" << m - (u * s * v.transpose()) << endl;
    //cout << endl;
    //cout << "det m: " << m.determinant() << endl;
    //cout << endl;
    //cout << "det m': " << (u * s * v.transpose()).determinant() << endl;
}


CINO_INLINE
void closest_pos_det_matrix(double T[3][3])
{
    Eigen::Matrix3d t;
    from_std_3x3_to_Eigen_3x3(T, t);

    if (t.determinant() > 0.0) return;

    Eigen::Matrix3d u, v;
    Eigen::DiagonalMatrix<double,3> s;

    ssvd(t, u, s, v);

    //cout << ":::::::::::::::::::::::::::::::::::::::::::" << endl;
    //cout << T[0][0] << "\t" << T[0][1] << "\t" << T[0][2] << endl;
    //cout << T[1][0] << "\t" << T[1][1] << "\t" << T[1][2] << endl;
    //cout << T[2][0] << "\t" << T[2][1] << "\t" << T[2][2] << endl;

    assert(s.diagonal()[0] > 0.0);
    assert(s.diagonal()[1] > 0.0);

    double avg_scale = (s.diagonal()[0] + s.diagonal()[1]) * 0.5;
    s.diagonal()[0] = avg_scale;
    s.diagonal()[1] = avg_scale;
    s.diagonal()[2] = avg_scale;

    t = u * s * v.transpose();

    assert(s.diagonal()[0] > 0.0);
    assert(s.diagonal()[1] > 0.0);
    assert(s.diagonal()[2] > 0.0);
    assert(t.determinant() > 0.0);

    from_eigen_3x3_to_std_3x3(t, T);
}

}
