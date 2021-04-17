// Copyright (c) 2018-2020 ISciences, LLC.
// All rights reserved.
//
// This software is licensed under the Apache License, Version 2.0 (the "License").
// You may not use this file except in compliance with the License. You may
// obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "raster_utils.h"

using Grid = exactextract::Grid<exactextract::bounded_extent>;

Grid make_grid(const Rcpp::S4 & rast) {
  Rcpp::Environment raster = Rcpp::Environment::namespace_env("raster");
  Rcpp::Function resFn = raster["res"];

  Rcpp::S4 extent = rast.slot("extent");
  Rcpp::NumericVector res = resFn(rast);

  return {{
    extent.slot("xmin"),
    extent.slot("ymin"),
    extent.slot("xmax"),
    extent.slot("ymax"),
    },
    res[0],
    res[1]
  };
}

int get_nlayers(Rcpp::S4 & rast) {
  Rcpp::Environment raster = Rcpp::Environment::namespace_env("raster");
  Rcpp::Function nlayersFn = raster["nlayers"];

  Rcpp::NumericVector nlayersVec = nlayersFn(rast);

  return static_cast<int>(nlayersVec[0]);
}

// Determine the column index in 'rast' for every column in 'grid'
Rcpp::IntegerVector cols_for_x(Rcpp::S4 & rast, Grid grid) {
  Rcpp::Environment raster = Rcpp::Environment::namespace_env("raster");
  Rcpp::Function colFromX = raster["colFromX"];

  // Since the coverage fraction grid is potentially higher-resolution than
  // the source raster grid, we take all x values from the coverage grid,
  // plug them into colFromX to find their column index, and then put the
  // column indices through xFromCol to get a (snapped) x value. This could
  // probably be done faster without calling R functions, but this approach
  // ensures consistency with raster package results.
  Rcpp::NumericVector x(grid.cols());

  for (size_t i = 0; i < grid.cols(); i++) {
    x[i] = grid.x_for_col(i);
  }

  Rcpp::IntegerVector cols = colFromX(rast, x);
  return Rcpp::rep(cols, grid.rows());
}

// Determine the row index in 'rast' for every row in 'grid'
Rcpp::IntegerVector rows_for_y(Rcpp::S4 & rast, Grid grid) {
  Rcpp::Environment raster = Rcpp::Environment::namespace_env("raster");
  Rcpp::Function rowFromY = raster["rowFromY"];

  Rcpp::NumericVector y(grid.rows());

  for (size_t i = 0; i < grid.rows(); i++) {
    y[i] = grid.y_for_row(i);
  }

  Rcpp::IntegerVector rows = rowFromY(rast, y);
  return Rcpp::rep_each(rows, grid.cols());
}

// Determine the x value in 'rast' for every column in 'grid'
// Although 'grid' can provide x values directly, this ensures consistency
// with values provided by the 'raster' package.
Rcpp::NumericVector get_x_values(Rcpp::S4 & rast, Grid grid) {
  Rcpp::Environment raster = Rcpp::Environment::namespace_env("raster");
  Rcpp::Function xFromCol = raster["xFromCol"];

  Rcpp::IntegerVector cols = cols_for_x(rast, grid);
  return xFromCol(rast, cols);
}

// Determine the y value in 'rast' for every row in 'grid'
// Although 'grid' can provide y values directly, this ensures consistency
// with values provided by the 'raster' package.
Rcpp::NumericVector get_y_values(Rcpp::S4 & rast, Grid grid) {
  Rcpp::Environment raster = Rcpp::Environment::namespace_env("raster");
  Rcpp::Function yFromRow = raster["yFromRow"];

  Rcpp::IntegerVector rows = rows_for_y(rast, grid);
  return yFromRow(rast, rows);
}

Rcpp::NumericVector get_cell_numbers(Rcpp::S4 & rast, Grid grid) {
  Rcpp::Environment raster = Rcpp::Environment::namespace_env("raster");
  Rcpp::Function cellFromRowCol = raster["cellFromRowCol"];

  Rcpp::IntegerVector cols = cols_for_x(rast, grid);
  Rcpp::IntegerVector rows = rows_for_y(rast, grid);

  return cellFromRowCol(rast, rows, cols);
}
