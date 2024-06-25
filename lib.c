#include"ext.h"
#include"ext_obex.h"
#include"z_dsp.h"
#include<simd/simd.h>
#include<Accelerate/Accelerate.h>
typedef struct {
	t_pxobject const super;
	double f;
	double e;
	double r;
	double v;
	enum {
		PHASOR,
		RADIAN,
	} m;
} t_kepler;
C74_HIDDEN t_class const * class = NULL;
C74_HIDDEN t_kepler const * const new(t_symbol const*const s, double const f, double const e) {
	if ( 0 <= e && e < 1 ) {
		t_kepler*const this = (t_kepler*const)object_alloc((t_class*const)class);
		if ( this ) {
			z_dsp_setup((t_pxobject*const)this, 2);
			outlet_new(this, "signal");
			outlet_new(this, "signal");
			this->f = f;
			this->e = e;
			this->r = 0;
			this->m = RADIAN;
		}
		return this;
	} else {
		error("2nd argument: Eccentricity must be between 0 and 1");
		return NULL;
	}
}
C74_HIDDEN void del(t_kepler*const this) {
	
}
C74_HIDDEN void empty(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
	
}
C74_HIDDEN void dspss(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
	register double const fs = (uintptr_t const)parameter;
	register double const f = this->f;
	register double const e = this->e;
	register double const a = fma(-e, e, 1);
	register double const b = sqrt(a);
	register double r = this->r;
	register double v = this->v;
	register double*q = alloca(2*sizeof(double const));
	for ( register long k = 0, K = length ; k < K ; ++ k ) {
		register double const l = a / fma(cospi(2 * r), e, 1);
		register double const g = b / l / l;
		o[0][k] = l;
		o[1][k] = r;
		r = modf(fma(g + v, f / fs / 2, r), q);
		v = g;
	}
	switch ( this->m ) {
		case RADIAN:
			q[0] = 2 * M_PI;
			q[1] =   - M_PI;
			vDSP_vsmsaD(o[1], 1, q+0, q+1, o[1], 1, length);
			break;
		case PHASOR:
			break;
	}
	this->r = r;
	this->v = v;
}
C74_HIDDEN void dspvs(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
	register double const fs = (uintptr_t const)parameter;
	register double const e = this->e;
	register double const a = fma(-e, e, 1);
	register double const b = sqrt(a);
	register double r = this->r;
	register double v = this->v;
	register double*q = alloca(2*sizeof(double const));
	for ( register long k = 0, K = length ; k < K ; ++ k ) {
		register double const f = i[0][k];
		register double const l = a / fma(cospi(2 * r), e, 1);
		register double const g = b / l / l;
		o[0][k] = l;
		o[1][k] = r;
		r = modf(fma(g + v, f / fs / 2, r), q);
		v = g;
	}
	switch ( this->m ) {
		case RADIAN:
			q[0] = 2 * M_PI;
			q[1] =   - M_PI;
			vDSP_vsmsaD(o[1], 1, q+0, q+1, o[1], 1, length);
			break;
		case PHASOR:
			break;
	}
	this->r = r;
	this->v = v;
}
C74_HIDDEN void dspsv(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
	register double const fs = (uintptr_t const)parameter;
	register double const f = this->f;
	register double r = this->r;
	register double v = this->v;
	register double*q = alloca(2*sizeof(double const));
	for ( register long k = 0, K = length ; k < K ; ++ k ) {
		register double const e = i[1][k];
		register double const a = fma(-e, e, 1);
		register double const b = sqrt(a);
		register double const l = a / fma(cospi(2 * r), e, 1);
		register double const g = b / l / l;
		o[0][k] = l;
		o[1][k] = r;
		r = modf(fma(g + v, f / fs / 2, r), q);
		v = g;
	}
	switch ( this->m ) {
		case RADIAN:
			q[0] = 2 * M_PI;
			q[1] =   - M_PI;
			vDSP_vsmsaD(o[1], 1, q+0, q+1, o[1], 1, length);
			break;
		case PHASOR:
			break;
	}
	this->r = r;
	this->v = v;
}
C74_HIDDEN void dspvv(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
	register double const fs = (uintptr_t const)parameter;
	register double r = this->r;
	register double v = this->v;
	register double*q = alloca(2 * sizeof(double const));
	for ( register long k = 0, K = length ; k < K ; ++ k ) {
		register double const f = i[0][k];
		register double const e = i[1][k];
		register double const a = fma(-e, e, 1);
		register double const b = sqrt(a);
		register double const l = a / fma(cospi(2 * r), e, 1);
		register double const g = b / l / l;
		o[0][k] = l;
		o[1][k] = r;
		r = modf(fma(g + v, f / fs / 2, r), q);
		v = g;
	}
	switch ( this->m ) {
		case RADIAN:
			q[0] = 2 * M_PI;
			q[1] =   - M_PI;
			vDSP_vsmsaD(o[1], 1, q+0, q+1, o[1], 1, length);
			break;
		case PHASOR:
			break;
	}
	this->r = r;
	this->v = v;
}
C74_HIDDEN void dsp64(t_kepler*const this, t_object const*const dsp64, short const*const count, double const samplerate, long const maxvectorsize, long const flags) {
	this->r = 0;
	this->v = sqrt(1 - this->e * this->e);
	if ( !count[2] && !count[3] )
		dsp_add64((t_object*const)dsp64, (t_object*const)this, (t_perfroutine64 const)empty, 0, (uintptr_t const)samplerate);
	else if ( count[0] && count[1] )
		dsp_add64((t_object*const)dsp64, (t_object*const)this, (t_perfroutine64 const)dspvv, 0, (uintptr_t const)samplerate);
	else if ( count[0] )
		dsp_add64((t_object*const)dsp64, (t_object*const)this, (t_perfroutine64 const)dspvs, 0, (uintptr_t const)samplerate);
	else if ( count[1] )
		dsp_add64((t_object*const)dsp64, (t_object*const)this, (t_perfroutine64 const)dspsv, 0, (uintptr_t const)samplerate);
	else
		dsp_add64((t_object*const)dsp64, (t_object*const)this, (t_perfroutine64 const)dspss, 0, (uintptr_t const)samplerate);
}
C74_HIDDEN void param(t_kepler*const this, double const value) {
	switch ( proxy_getinlet((t_object*const)this) ) {
		case 0:
			this->f = value;
			break;
		case 1:
			if ( value < 0 )
				object_error((t_object*const)this, "[%s] eccentricity should be between 0 and 1", class->c_sym->s_name);
			else if ( value < 1 )
				this->e = value;
			else
				object_error((t_object*const)this, "[%s] eccentricity should be between 0 and 1", class->c_sym->s_name);
			break;
		default:
			object_error((t_object*const)this, "[%s] fatal error", class->c_sym->s_name);
			break;
	}
}
C74_HIDDEN void scale(t_kepler*const this, t_symbol const*const symbol) {
	if ( !symbol );
	else if ( symbol == gensym("phasor") )
		this->m = PHASOR;
	else if ( symbol == gensym("radian") )
		this->m = RADIAN;
	else
		object_error((t_object*const)this, "[%s] argument must be phasor or radian", class->c_sym->s_name, symbol->s_name);
}
C74_EXPORT void ext_main(void*const _) {
	if (!class) {
		t_class * const object = (t_class*const)class_new("keplerosc~", (method const)new, (method const)del, sizeof(t_kepler const), 0L, A_DEFFLOAT, A_DEFFLOAT, 0);
		class_addmethod(object, (method const)param, "float", A_FLOAT, 0);
		class_addmethod(object, (method const)dsp64, "dsp64", A_CANT, 0);
		class_addmethod(object, (method const)scale, "scale", A_SYM, 0);
		class_dspinit(object);
		class_register(CLASS_BOX, object);
		class = object;
	}
}

