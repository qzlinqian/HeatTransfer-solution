//
// Created by Qian Lin on 2018/12/10.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

int main(){
  double dx = 0.08f, dy = 0.08f;
  double h = 40.0f, t_f = 25.0f, lambda = 45.0f, q_w = 1500;

  std::vector<std::vector<double> > A;
  std::vector<double> c(24,0);
  std::vector<double> t(24,200);
  A.resize(24);
  for (int i=0;i<24;i++){
    A[i].resize(24,0);
  }

  // Inner points
  for (int i=0;i<24;i++){
    if ((i%6 == 0) || (i%6 == 5)) continue;
    if ((i/6 == 0) || (i/6 == 3)) continue;
    A[i][i-1] = 0.25f;
    A[i][i+1] = 0.25f;
    A[i][i-6] = 0.25f;
    A[i][i+6] = 0.25f;
  }

  // Convection
  double two_h_x_l = 2.0f * h * dx / lambda;
  A[11][5] = 1.0f / (4.0f + two_h_x_l);
  A[11][17] = A[11][5];
  A[11][10] = 2 * A[11][5];
  c[11] = two_h_x_l / (4.0f + two_h_x_l) * t_f;
  A[17][11] = A[11][5];
  A[17][23] = A[11][17];
  A[17][16] = A[11][10];
  c[17] = c[11];

  // Insulation
  A[6][0] = 0.25f;
  A[6][12] = 0.25f;
  A[6][7] = 0.5f;
  A[12][6] = 0.25f;
  A[12][18] = 0.25f;
  A[12][13] = 0.5f;

  // I
  c[0] = 200.0f;
  c[1] = 200.0f;
  c[2] = 200.0f;
  c[3] = 200.0f;
  c[4] = 200.0f;
  c[5] = 200.0f;

  // II
  double temp = dx * q_w / lambda / 2;
  for (int i=19;i<23;i++){
    A[i][i-6] = 0.5f;
    A[i][i-1] = 0.25f;
    A[i][i+1] = 0.25f;
    c[i] = temp;
  }

  // half II + half iso
  A[18][12] = 0.5f;
  A[18][19] = 0.5f;
  c[18] = q_w * dx / lambda / 2;

  // half II + half III
  A[23][22] = 1.0f / (2.0 + h * dx / lambda);
  A[23][17] = A[23][22];
  c[23] = (q_w * dx / lambda + h * dx * t_f / lambda) / (2.0 + h * dx / lambda);


  // Gaussian Seidel Method
  std::vector<double> x_aux(24,0);
  double error;

  do {
    error = 0;
    for (int i=0;i<24;i++){
      x_aux[i] = t[i];
      t[i] = 0;
      for (int j=0;j<24;j++){
        t[i] += A[i][j] * x_aux[j];
      }
      t[i] += c[i];
      double temp = std::abs(x_aux[i] - t[i]);
      if (temp > error)
        error = temp;
    }
  } while (error>0.0001);

//  for (int i=0;i<24;i++){
//    std::cout<<t[i]<<" ";
//    if (i%6 == 5) std::cout<<std::endl;
//  }

  // To file
  std::ofstream file;
  file.open("data.dat");
  file<<"# X Y Z\n";
  for (int i=0;i<24;i++){
    file << i%6 * 80 << " " << i/6 * 80 << " " << t[i] << "\n";
  }
  file.close();

  return 0;
}

