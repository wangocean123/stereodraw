#include "stdafx.h"
#include "curve.h"
#include "geo_algorithom.h"

namespace Curve
{
	template<class Type>
	class auto_ptr
	{
	public:
		auto_ptr(Type *p, bool barray=true)
		{
			m_p = p;
			m_barray = barray;
		}
		~auto_ptr()
		{
			if (m_barray)
				delete[] m_p;
			else
				delete m_p;
		}
		Type *m_p;
		bool m_barray;
	};

	TriDiagonalMatrixF::TriDiagonalMatrixF(int n)
	{
		N = n;
		A = new double[n];
		B = new double[n];
		C = new double[n];
	}

	TriDiagonalMatrixF::~TriDiagonalMatrixF()
	{
		if (A)delete[] A;
		if (B)delete[] B;
		if (C)delete[] C;
	}

	bool TriDiagonalMatrixF::Solve(double* d, double *r)
	{
		int n = N;

		// cPrime
		double* cPrime = new double[n];
		cPrime[0] = C[0] / B[0];

		for (int i = 1; i < n; i++)
		{
			cPrime[i] = C[i] / (B[i] - cPrime[i - 1] * A[i]);
		}

		// dPrime
		double* dPrime = new double[n];
		dPrime[0] = d[0] / B[0];

		for (int i = 1; i < n; i++)
		{
			dPrime[i] = (d[i] - dPrime[i - 1] * A[i]) / (B[i] - cPrime[i - 1] * A[i]);
		}

		// Back substitution
		double* x = new double[n];
		x[n - 1] = (double)dPrime[n - 1];

		for (int i = n - 2; i >= 0; i--)
		{
			x[i] = (double)(dPrime[i] - cPrime[i] * x[i + 1]);
		}

		memcpy(r, x, sizeof(double)*n);

		delete[] cPrime;
		delete[] dPrime;
		delete[] x;

		return true;
	}

	bool TriDiagonalMatrixF::Solve2(double* d, double *r)
	{
		int n = N;
		double* r3 = new double[n];

		//根据每行只有4个非零元素的特点，直接采用消元法接方程组，经过测试结果正确。速度与资源占用都很好
		if (true)
		{
			double* d2 = new double[n];
			double* A2 = new double[n];
			double* B2 = new double[n];
			double* C2 = new double[n];
			memcpy(d2, d, sizeof(double)*n);
			memcpy(A2, A, sizeof(double)*n);
			memcpy(B2, B, sizeof(double)*n);
			memcpy(C2, C, sizeof(double)*n);

			//记录最后一列和最后一行，计算它们时，需要迭代，所以全部记录下来
			double* last_col = new double[n];
			double* last_row = new double[n];

			for (int i = 0; i < n; i++) last_col[i] = 0;
			last_col[0] = A[0];
			last_col[n - 2] = C[n - 2];
			last_col[n - 1] = B[n - 1];

			for (int i = 0; i < n; i++) last_row[i] = 0;
			last_row[0] = C[n - 1];
			last_row[n - 2] = A[n - 1];
			last_row[n - 1] = B[n - 1];

			for (int i = 0; i < n - 1; i++)
			{
				//double k = -A[1] / B[0];
				//A[1] += k * B[0];
				//B[1] += k * C[0];
				//last_col[1] += k * last_col[0];

				//最后一行放入到 last_row 的处理中，所以跳过
				//if(i!=(n-2))
				{
					double k = -A[i + 1] / B[i];
					A[i + 1] += k * B[i];
					B[i + 1] += k * C[i];
					last_col[i + 1] += k * last_col[i];
					d[i + 1] += k * d[i];
				}

				//k = -last_row[0] / B[0];
				//last_row[0] += k * B[0];
				//last_row[1] += k * C[0];

				if (true)
				{
					double k = -last_row[i] / B[i];
					last_row[i] += k * B[i];

					//i=n-2时，last_row[i + 1]和last_row[n - 1]相同，此处避免重复计算
					if (i != (n - 2))
						last_row[i + 1] += k * C[i];
					last_row[n - 1] += k * last_col[i];
					d[n - 1] += k * d[i];
				}
			}

			//last_col[n - 2]清零后，下面通过r[i+1]计算r[i]就可以采用一致的公式进行计算了；此处下面的代码执行之前就应该有C[n - 2] == last_col[n - 2]
			C[n - 2] = last_col[n - 2]; last_col[n - 2] = 0;
			B[n - 1] = last_row[n - 1];

			r3[n - 1] = d[n - 1] / B[n - 1];

			for (int i = n - 2; i >= 0; i--)
			{
				r3[i] = (d[i] - last_col[i] * r3[n - 1] - r3[i + 1] * C[i]) / B[i];
			}

			memcpy(d, d2, sizeof(double)*n);
			memcpy(A, A2, sizeof(double)*n);
			memcpy(B, B2, sizeof(double)*n);
			memcpy(C, C2, sizeof(double)*n);

			delete[] last_col;
			delete[] last_row;
			delete[] d2;
			delete[] A2;
			delete[] B2;
			delete[] C2;
		}

		memcpy(r, r3, sizeof(double)*n);
		delete[] r3;

		return true;
	}

	CCubicSpline::CCubicSpline()
	{
		a = NULL;
		b = NULL;
		N = 0;
		xOrig = NULL;
		yOrig = NULL;
		nOutNum = 0;
	}


	CCubicSpline::~CCubicSpline()
	{
		ClearMems();
	}

	int CCubicSpline::GetNextXIndex(int last_knot_index, double x)
	{
		if (x < xOrig[last_knot_index])
		{
			// error;
		}

		while ((last_knot_index < N - 2) && (x > xOrig[last_knot_index + 1]))
		{
			last_knot_index++;
		}

		return last_knot_index;
	}

	double CCubicSpline::EvalSpline(double x, int j, bool bclosed)
	{
		double dx = xOrig[j + 1] - xOrig[j];
		double t = (x - xOrig[j]) / dx;
		double y = 0;
		if (bclosed && j == (N - 1))
			y = (1 - t) * yOrig[j] + t * yOrig[j + 1] + t * (1 - t) * (a[0] * (1 - t) + b[0] * t); // equation 9
		else
			y = (1 - t) * yOrig[j] + t * yOrig[j + 1] + t * (1 - t) * (a[j] * (1 - t) + b[j] * t); // equation 9

		return y;
	}

	bool CCubicSpline::FitAndEval(double* x, double* y, double* xs, bool bclosed, double *ret)
	{
		if (!Fit(x, y, bclosed))
			return false;

		Eval(xs, ret, bclosed, NULL);
		return true;
	}

	bool CCubicSpline::Fit(double* x, double* y, bool bclosed)
	{
		xOrig = new double[N];
		yOrig = new double[N];

		// Save x and y for eval
		memcpy(xOrig, x, sizeof(double)*N);
		memcpy(yOrig, y, sizeof(double)*N);

		int n = N;
		double* r = new double[bclosed ? (n - 1) : n]; // the right hand side numbers: wikipedia page overloads b

		TriDiagonalMatrixF m(bclosed ? (n - 1) : n);
		double dx1, dx2, dy1, dy2;

		// First row is different (equation 16 from the article)
		if (!bclosed) //natural spline
		{
			dx1 = x[1] - x[0];
			m.C[0] = 1.0f / dx1;
			m.B[0] = 2.0f * m.C[0];
			r[0] = 3 * (y[1] - y[0]) / (dx1 * dx1);
		}
		else
		{
			dx1 = x[n - 1] - x[n - 2];
			dx2 = x[1] - x[0];

			m.A[0] = 1.0f / dx1;
			m.C[0] = 1.0f / dx2;
			m.B[0] = 2.0f * (m.A[0] + m.C[0]);

			dy1 = y[n - 1] - y[n - 2];
			dy2 = y[1] - y[0];
			r[0] = 3 * (dy1 / (dx1 * dx1) + dy2 / (dx2 * dx2));
		}

		// Body rows (equation 15 from the article)
		for (int i = 1; i < n - 1; i++)
		{
			dx1 = x[i] - x[i - 1];
			dx2 = x[i + 1] - x[i];

			m.A[i] = 1.0f / dx1;
			m.C[i] = 1.0f / dx2;
			m.B[i] = 2.0f * (m.A[i] + m.C[i]);

			dy1 = y[i] - y[i - 1];
			dy2 = y[i + 1] - y[i];
			r[i] = 3 * (dy1 / (dx1 * dx1) + dy2 / (dx2 * dx2));
		}

		// Last row also different (equation 17 from the article) //natural spline
		// for closed spline, Last row is the same to body rows
		if (!bclosed)
		{
			dx1 = x[n - 1] - x[n - 2];
			dy1 = y[n - 1] - y[n - 2];
			m.A[n - 1] = 1.0f / dx1;
			m.B[n - 1] = 2.0f * m.A[n - 1];
			r[n - 1] = 3 * (dy1 / (dx1 * dx1));
		}

		// k is the solution to the matrix
		double *k = new double[bclosed ? (n - 1) : n];
		memset(k, 0, sizeof(double)*(bclosed ? (n - 1) : n));

		if (bclosed)
		{
			m.Solve2(r, k);
		}
		else
		{
			m.Solve(r, k);
		}

		// a and b are each spline's coefficients
		a = new double[n - 1];
		b = new double[n - 1];

		for (int i = 1; i < n; i++)
		{
			dx1 = x[i] - x[i - 1];
			dy1 = y[i] - y[i - 1];
			a[i - 1] = k[i - 1] * dx1 - dy1; // equation 10 from the article
			if (!bclosed)
				b[i - 1] = -k[i] * dx1 + dy1; // equation 11 from the article
			else
				b[i - 1] = -k[i % (n - 1)] * dx1 + dy1;
		}

		delete[] r;
		delete[] k;

		return true;
	}

	void CCubicSpline::Eval(double* x, double *ret, bool bclosed, int *knot_insert_indexs)
	{
		int n = N;
		int lastIndex = 0; // Reset simultaneous traversal in case there are multiple calls

		int nknot_index = 0;

		for (int i = 0; i < nOutNum; i++)
		{
			// Find which spline can be used to compute this x (by simultaneous traverse)
			int j = GetNextXIndex(lastIndex, x[i]);

			if (knot_insert_indexs)
			{
				if (i == 0)
				{
					knot_insert_indexs[nknot_index++] = i;
				}
				//移动到了下一个顶点
				else if (j != lastIndex)
				{
					knot_insert_indexs[nknot_index++] = i;
				}
			}


			// Evaluate using j'th spline
			ret[i] = EvalSpline(x[i], j, bclosed);

			lastIndex = j;
		}

		if (knot_insert_indexs)
		{
			knot_insert_indexs[nknot_index] = nOutNum;
		}
	}

	void CCubicSpline::ClearMems()
	{
		if (a)delete[] a;
		if (b)delete[] b;
		if (xOrig)delete[] xOrig;
		if (yOrig)delete[] yOrig;

		a = NULL;
		b = NULL;
		xOrig = NULL;
		yOrig = NULL;
	}

	bool CCubicSpline::FitGeometric(double* x, double* y, int nInput, int nOutputPoints, bool bclosed, double* xs, double* ys)
	{
		double *x2, *y2;
		N = nInput;
		x2 = new double[N];
		y2 = new double[N];
		memcpy(x2, x, sizeof(double)*nInput);
		memcpy(y2, y, sizeof(double)*nInput);

		// Compute distances
		int n = N;
		double* dists = new double[n]; // cumulative distance
		dists[0] = 0;
		double totalDist = 0;

		for (int i = 1; i < n; i++)
		{
			double dx = x2[i] - x2[i - 1];
			double dy = y2[i] - y2[i - 1];
			double dist = (double)sqrt(dx * dx + dy * dy);
			totalDist += dist;
			dists[i] = totalDist;
		}

		// Create 'times' to interpolate to
		double dt = totalDist / (nOutputPoints - 1);
		double* times = new double[nOutputPoints];
		times[0] = 0;

		for (int i = 1; i < nOutputPoints; i++)
		{
			times[i] = times[i - 1] + dt;
		}

		nOutNum = nOutputPoints;

		int *knot_insert_indexs = new int[nInput];
		memset(knot_insert_indexs, 0, sizeof(int)*nInput);

		// Spline fit both x and y to times
		if (!Fit(dists, x2, bclosed))
			return false;

		Eval(times, xs, bclosed, knot_insert_indexs);
		ClearMems();
		delete[] knot_insert_indexs;

		int *knot_insert_indexs2 = new int[nInput];
		memset(knot_insert_indexs2, 0, sizeof(int)*nInput);

		if (!Fit(dists, y2, bclosed))
			return false;

		Eval(times, ys, bclosed, knot_insert_indexs2);
		ClearMems();
		delete[] knot_insert_indexs2;

		delete[] times;
		delete[] dists;
		delete[] x2;
		delete[] y2;

		return true;
	}
	
	bool CCubicSpline::FitGeometric(const StereoDraw::geo_vertex *pts, int nInput, double toler, bool bclosed, StereoDraw::vertex_array& ret_pts)
	{
		int nInput0 = nInput;

		StereoDraw::geo_vertex *pts_new = new StereoDraw::geo_vertex[nInput];
		auto_ptr<StereoDraw::geo_vertex> auto_p1(pts_new);

		//kick off repeat knots
		if (true)
		{			
			int ref_index = 0;
			pts_new[ref_index] = pts[0];
			for (int i = 1; i < nInput; i++)
			{
				if ((pts[i].x - pts_new[ref_index].x)*(pts[i].x - pts_new[ref_index].x) + (pts[i].y - pts_new[ref_index].y)*(pts[i].y - pts_new[ref_index].y) < 1e-8)
				{
					pt_3d_repeat pt;
					pt.pt = pts[i];
					pt.repeat_index = ref_index;
					m_repeat_knots.push_back(pt);
				}
				else
				{
					ref_index++;
					pts_new[ref_index] = pts[i];
				}
			}

			nInput = ref_index + 1;
			pts = pts_new;
		}

		double *x2, *y2;
		N = nInput;
		x2 = new double[N];
		y2 = new double[N];
		auto_ptr<double> auto_p2(x2);
		auto_ptr<double> auto_p3(y2);

		for (int i = 0; i < N; i++)
		{
			x2[i] = pts[i].x;
			y2[i] = pts[i].y;
		}

		// Compute distances
		int n = N;
		double* dists = new double[n]; // cumulative distance
		auto_ptr<double> auto_p4(dists);

		dists[0] = 0;
		double totalDist = 0;

		for (int i = 1; i < n; i++)
		{
			double dx = x2[i] - x2[i - 1];
			double dy = y2[i] - y2[i - 1];
			double dist = (double)sqrt(dx * dx + dy * dy);
			totalDist += dist;
			dists[i] = totalDist;
		}

		//以平均弦长为直径的圆来估算内插点数
		int nOutputPoints = 0;
		double r = 0.5 * totalDist / (n-1);
		if (r < toler * 5)r = toler * 5;
		nOutputPoints = totalDist / (r*2*acos(1-toler/r));

		// Create 'times' to interpolate to
		double dt = totalDist / (nOutputPoints - 1);
		double* times = new double[nOutputPoints];
		auto_ptr<double> auto_p5(times);
		times[0] = 0;

		for (int i = 1; i < nOutputPoints; i++)
		{
			times[i] = times[i - 1] + dt;
		}

		nOutNum = nOutputPoints;

		double *xs = new double[nOutputPoints];
		double *ys = new double[nOutputPoints];
		auto_ptr<double> auto_p6(xs);
		auto_ptr<double> auto_p7(ys);

		//record knot insert position
		int *knot_insert_indexs = new int[N];
		auto_ptr<int> auto_p8(knot_insert_indexs);
		memset(knot_insert_indexs, 0, sizeof(int)*N);

		// Spline fit both x and y to times
		if (!Fit(dists, x2, bclosed))
		{
			delete[] pts_new;
			return false;
		}
		
		Eval(times, xs, bclosed, knot_insert_indexs);
		ClearMems();

		int *knot_insert_indexs2 = new int[N];
		auto_ptr<int> auto_p9(knot_insert_indexs2);
		memset(knot_insert_indexs2, 0, sizeof(int)*N);

		if (!Fit(dists, y2, bclosed))
		{
			return false;
		}

		Eval(times, ys, bclosed, knot_insert_indexs2);
		ClearMems();

		ret_pts.resize(nOutputPoints + nInput0);

		//插入节点，包括重复节点
		int m = 0;
		int repeat_i = 0;
		for (int i = 0; i < nInput-1; i++)
		{
			ret_pts[m++] = pts[i];
			for (; repeat_i < m_repeat_knots.size(); repeat_i++)
			{
				if (m_repeat_knots[repeat_i].repeat_index == i)
				{
					ret_pts[m++] = m_repeat_knots[repeat_i].pt;
				}
				else break;
			}

			int start = knot_insert_indexs[i];
			int end = knot_insert_indexs[i+1];

			double z0 = ret_pts[m - 1].z;
			double dz = pts_new[i + 1].z - z0;

			for (int j = start; j <end; j++)
			{
				ret_pts[m].x = xs[j];
				ret_pts[m].y = ys[j];
				ret_pts[m].z = z0 + (j-start)*dz / (end - start);
				m++;
			}
		}

		ret_pts[m++] = pts[nInput - 1];
		for (; repeat_i < m_repeat_knots.size(); repeat_i++)
		{
			if (m_repeat_knots[repeat_i].repeat_index == (nInput - 1))
			{
				ret_pts[m++] = m_repeat_knots[repeat_i].pt;
			}
			else break;
		}

		if (m != (nOutputPoints + nInput0) )
		{
			//error
			int a = 1;
		}

		return true;
	}

	void Arc(StereoDraw::geo_vertex pt1, StereoDraw::geo_vertex pt2, StereoDraw::geo_vertex pt3, double toler, StereoDraw::vertex_array& ret_pts)
	{
		//计算圆心/半径，方法：通过两个线段的中垂线交点
		StereoDraw::geo_vertex mpt1, mpt2;
		mpt1 = geo_api::get_mid_2d(pt1, pt2);
		mpt2 = geo_api::get_mid_2d(pt2, pt3);

		StereoDraw::geo_vertex mline_pt1, mline_pt2, mline_pt3, mline_pt4, cpt;
		geo_api::vector<StereoDraw::geo_vertex> v1(pt1, pt2), v2(pt2, pt3);
		
		mline_pt1 = mline_pt2 = mpt1;
		v1.rotate90();
		v1.add_to(mline_pt1);
		v1.rotate180();
		v1.add_to(mline_pt2);

		mline_pt3 = mline_pt4 = mpt2;
		v2.rotate90();
		v2.add_to(mline_pt3);
		v2.rotate180();
		v2.add_to(mline_pt4);

		if (!geo_api::get_intersect_of_lines(mline_pt1, mline_pt2, mline_pt3, mline_pt4,&cpt,NULL))
		{
			ret_pts.push_back(pt1);
			ret_pts.push_back(pt2);
			ret_pts.push_back(pt3);
		}

		double r = geo_api::get_dis_2d(pt1, cpt);
		if (toler/r>0.2)
		{
			toler = r*0.2;
		}

		double a = 2 * acos(1 - toler / r);
		double a1 = geo_api::get_angle(cpt, pt1);
		double a2 = geo_api::get_angle(cpt, pt2);
		double a3 = geo_api::get_angle(cpt, pt3);

		if (a1 < a3)
		{
			if (a2>a1 && a2 < a3);//顺序
			else if (a2>a3)
			{
				a = -a;
				a2 -= 2 * geo_api::PI;
				a3 -= 2 * geo_api::PI;
			}
			else if (a2 < a1)
			{
				a = -a;
				a3 -= 2 * geo_api::PI;
			}
		}
		else if (a1>a3)
		{
			if (a2 > a3 && a2 < a1)
			{
				a = -a;
			}
			else if (a2>a1)
			{
				a3 += 2 * geo_api::PI;
			}
			else if (a2 < a3)
			{
				a2 += 2 * geo_api::PI;
				a3 += 2 * geo_api::PI;
			}
		}

		ret_pts.push_back(pt1);
		double t = a1 + a;

		double dz = pt2.z - pt1.z;
		bool increased = (a > 0);
		for (; increased ? (t < a2) : (t>a2); t += a)
		{
			StereoDraw::geo_vertex pt;
			pt.x = cpt.x + r*cos(t);
			pt.y = cpt.y + r*sin(t);
			pt.z = pt1.z + dz*(t - a1) / (a2 - a1);
			ret_pts.push_back(pt);
		}
		ret_pts.push_back(pt2);

		dz = pt3.z - pt2.z;
		for (; increased ? (t < a3) : (t>a3); t += a)
		{
			StereoDraw::geo_vertex pt;
			pt.x = cpt.x + r*cos(t);
			pt.y = cpt.y + r*sin(t);
			pt.z = pt2.z + dz*(t - a2) / (a3 - a2);
			ret_pts.push_back(pt);
		}
		ret_pts.push_back(pt3);
	}

	void Curve(const StereoDraw::geo_vertex *pts, int nInput, double toler, bool bclosed, StereoDraw::vertex_array& ret_pts)
	{
		if (nInput == 2)
		{
			for (int i = 0; i < nInput; i++)
			{
				ret_pts.push_back(pts[i]);
			}
			return;
		}

		int start = 0;
		for (int i = 2; i < nInput+1; i++)
		{
			if (i == nInput - 1 || pts[i].pen_code != pts[i - 1].pen_code)
			{
				if (i == (nInput - 1) && pts[i].pen_code == pts[i - 1].pen_code)
					i++;

				int code = pts[i - 1].pen_code;

				if (code == StereoDraw::geo_vertex::code_arc_3p)
				{
					int j = start;
					for (; j < i;)
					{
						if ((i - j)>2)
						{
							Arc(pts[j], pts[j + 1], pts[j + 2], toler, ret_pts);
							j += 2;
						}
						else
						{
							ret_pts.push_back(pts[j]);
							j++;
						}
					}					
				}
				else if (code == StereoDraw::geo_vertex::code_spline)
				{
					int j = start;
					if ((i - j)>2)
					{
						CCubicSpline spline;
						StereoDraw::vertex_array pts2;
						bool bclosed = false;

						if (i == (nInput - 1) && j == 0 && geo_api::is_equal_point(pts[i], pts[j]))
							bclosed = true;

						spline.FitGeometric(pts + j, i - j, toler, bclosed, pts2);
						ret_pts.insert(ret_pts.begin(), pts2.begin(), pts2.end());
					}
					else
					{
						for (; j<i; i++)
						{
							ret_pts.push_back(pts[j]);
						}
					}
				}
				else
				{
					for (int j=start; j<i; i++)
					{
						ret_pts.push_back(pts[j]);
					}
				}

				start = i;
			}
		}
	}

	void GetVertexIndex(const StereoDraw::vertex_array& ret_pts, std::vector<int>& indexs)
	{
		int size = ret_pts.size();
		for (int i = 0; i < size; i++)
		{
			if (ret_pts[i].pen_code != StereoDraw::geo_vertex::code_none)
			{
				indexs.push_back(i);
			}
		}
	}
}