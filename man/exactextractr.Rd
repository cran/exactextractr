% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/package.R
\docType{package}
\name{exactextractr}
\alias{exactextractr}
\title{exactextractr}
\description{
exactextractr quickly and accurately summarizes raster values over polygonal
areas, commonly referred to as \emph{zonal statistics}. It provides the following
functions:
\itemize{
\item \code{\link{exact_extract}} generates a data frame of grid cell values
                                  and the fraction of the cell's area  that is
                                  covered by a polygon. It can also compute
                                  statistics on these values without returning them
                                  directly.
\item \code{\link{coverage_fraction}} generates a raster whose values represent
                                      the fraction of each grid cell (0-1)
                                      covered by a polygon.
}
}
\author{
Daniel Baston
}
