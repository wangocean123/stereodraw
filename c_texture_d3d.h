#pragma once
#include "c_texture_obj.h"
#include "..\D3D\d3d.h"
#include "..\D3D\d3dx9core.h"

namespace StereoDraw
{
	class c_drawer_device_d3d;
	class STEREODRAW_API c_texture_d3d : public c_texture_obj
	{
	public:
		enum e_type
		{
			typeRender = 0,
			typeDynamic = 1
		};
		struct MyTexVertex
		{
			float x, y, z;
			DWORD color;
			float u, v;
		};
	public:
		c_texture_d3d();
		virtual ~c_texture_d3d();

		virtual bool create(int wid, int hei);
		virtual bool write_bitmap(HBITMAP hbmp);
		virtual void del();
		virtual void draw(const quad_points& qpt)const;
		virtual bool read_bitmap(HBITMAP hbmp);

		void set_device(c_drawer_device_d3d* p);
		bool can_render()const;

	private:
		LPDIRECT3DDEVICE9 device()const;
	public:

		c_drawer_device_d3d *m_pdevice;
		LPDIRECT3DTEXTURE9  m_ptexture;
		bool m_can_render;
	};
}