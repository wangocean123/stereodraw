#pragma once

#include <vector>
#include "point_def.h"

namespace Curve
{
	class TriDiagonalMatrixF
	{
	public:
		/// <summary>
		/// The values for the sub-diagonal. A[0] is never used.
		/// </summary>
		double * A;

		/// <summary>
		/// The values for the main diagonal.
		/// </summary>
		double * B;

		/// <summary>
		/// The values for the super-diagonal. C[C.Length-1] is never used.
		/// </summary>
		double * C;

		/// <summary>
		/// The width and height of this matrix.
		/// </summary>
		int N;

		/// <summary>
		/// Construct an NxN matrix.
		/// </summary>
		TriDiagonalMatrixF(int n);
		~TriDiagonalMatrixF();

		/// <summary>
		/// Solve the system of equations this*x=d given the specified d.
		/// </summary>
		/// <remarks>
		/// Uses the Thomas algorithm described in the wikipedia article: http://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
		/// Not optimized. Not destructive.
		/// </remarks>
		/// <param name="d">Right side of the equation.</param>
		bool Solve(double* d, double *r);

		//Equation: a1*xn + b1*x1 + c1*x2 = d1; the last one is: cn*x1 + an*x[n-1] + bn*xn = dn;
		/* coefficients matrix:
		b1  c1  ................................... a1   d1
		a2  b2  c2 ...................................   d2
		    a3  b3  c3 ...............................   d3



		                     an-3 bn-3 cn-3...........   dn-3
		                          an-2 bn-2 cn-2......   dn-2
		                               an-1 bn-1  cn-1   dn-1
		cn  ............................... an    bn     dn
		*/
		bool Solve2(double* d, double *r);
	};

	class STEREODRAW_API CCubicSpline
	{
	private:
		struct pt_3d_repeat
		{
			StereoDraw::geo_vertex pt;
			int repeat_index;
		};
	public:
		CCubicSpline();
		~CCubicSpline();

		bool FitGeometric(double* x, double* y, int nInput, int nOutputPoints, bool bclosed, double* xs, double* ys);
		bool FitGeometric(const StereoDraw::geo_vertex *pts, int nInput, double toler, bool bclosed, StereoDraw::vertex_array& ret_pts);

	private:
		int GetNextXIndex(int last_knot_index, double x);

		double EvalSpline(double x, int j, bool bclosed);

		bool FitAndEval(double* x, double* y, double* xs, bool bclosed, double *ret);

		bool Fit(double* x, double* y, bool bclosed);

		void Eval(double* x, double *ret, bool bclosed, int *knot_insert_indexs);

		void ClearMems();

	private:
		// N-1 spline coefficients for N points
		double* a;
		double* b;

		// Save the original x and y for Eval
		double* xOrig;
		double* yOrig;

		int N;
		int nOutNum;

		std::vector<pt_3d_repeat> m_repeat_knots;
	};

	
	void STEREODRAW_API Arc(StereoDraw::geo_vertex pt1, StereoDraw::geo_vertex pt2, StereoDraw::geo_vertex pt3, double toler, StereoDraw::vertex_array& ret_pts);

	void STEREODRAW_API Curve(const StereoDraw::geo_vertex *pts, int nInput, double toler, bool bclosed, StereoDraw::vertex_array& ret_pts);

	void STEREODRAW_API GetVertexIndex(const StereoDraw::vertex_array& ret_pts, std::vector<int>& indexs);
}