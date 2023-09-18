#include "ext.h"			// standard Max include, always required (except in Jitter)
#include "ext_obex.h"		// required for "new" style objects
#include "z_dsp.h"			// required for MSP objects
#include<Accelerate/Accelerate.h>
#include<simd/simd.h>
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

t_class const * class = NULL;

t_kepler const * const kepler_new(t_symbol const*const s, double const f, double const e) {
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

void kepler_free(t_kepler*const this) {
    
}

void kepler_empty(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
    
}

void kepler_dspss(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
    register double const fs = (uintptr_t const)parameter;
    register double const f = this->f;
    register double const e = this->e;
    register double const a = fma(-e, e, 1);
    register double const b = sqrt(a);
    register double r = this->r;
    register double v = this->v;
    register double*q = alloca(2*sizeof(double));
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

void kepler_dspvs(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
    register double const fs = (uintptr_t const)parameter;
    register double const e = this->e;
    register double const a = fma(-e, e, 1);
    register double const b = sqrt(a);
    register double r = this->r;
    register double v = this->v;
    register double*q = alloca(2*sizeof(double));
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

void kepler_dspsv(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
    register double const fs = (uintptr_t const)parameter;
    register double const f = this->f;
    register double r = this->r;
    register double v = this->v;
    register double*q = alloca(2*sizeof(double));
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

void kepler_dspvv(t_kepler*const this, t_object const*const dsp64, double const*const*const i, long const ic, double*const*const o, long const oc, long const length, long const flags, void*const parameter) {
    register double const fs = (uintptr_t const)parameter;
    register double r = this->r;
    register double v = this->v;
    register double*q = alloca(2 * sizeof(double));
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

void kepler_dsp64(t_kepler*const this, t_object const*const dsp64, short const*const count, double const samplerate, long const maxvectorsize, long const flags) {
    t_symbol const * const symbol = gensym("dsp_add64");
    uintptr_t const parameter = samplerate;
    this->r = 0;
    this->v = sqrt(1 - this->e * this->e);
    if ( !count[2] && !count[3] )
        object_method(dsp64, symbol, this, kepler_empty, 0, parameter);
    else if ( count[0] && count[1] )
        object_method(dsp64, symbol, this, kepler_dspvv, 0, parameter);
    else if ( count[0] )
        object_method(dsp64, symbol, this, kepler_dspvs, 0, parameter);
    else if ( count[1] )
        object_method(dsp64, symbol, this, kepler_dspsv, 0, parameter);
    else
        object_method(dsp64, symbol, this, kepler_dspss, 0, parameter);
}

void kepler_float(t_kepler*const this, double const value) {
    switch ( proxy_getinlet((t_object*const)this) ) {
        case 0:
            this->f = value;
            break;
        case 1:
            if ( value < 0 )
                error("[%s] eccentricity should be between 0 and 1", class->c_sym->s_name);
            else if ( value < 1 )
                this->e = value;
            else
                error("[%s] eccentricity should be between 0 and 1", class->c_sym->s_name);
            break;
        default:
            error("[%s] fatal error", class->c_sym->s_name);
            break;
    }
    post("%lf, %lf", this->f, this->e);
}

void kepler_scale(t_kepler*const this, t_symbol const*const symbol) {
    if ( !symbol );
    else if ( symbol == gensym("phasor") )
        this->m = PHASOR;
    else if ( symbol == gensym("radian") )
        this->m = RADIAN;
    else
        error("[%s] argument must be phasor or radian", class->c_sym->s_name, symbol->s_name);
}

void ext_main(void*const r) {
    if ((class = (t_class*const)class_new("kepler~", (method)kepler_new, (method)kepler_free, (long)sizeof(t_kepler), 0L, A_DEFFLOAT, A_DEFFLOAT, 0))) {
        class_addmethod((t_class*const)class, (method const)kepler_float, "float", A_FLOAT, 0);
        class_addmethod((t_class*const)class, (method const)kepler_dsp64, "dsp64", A_CANT, 0);
        class_addmethod((t_class*const)class, (method const)kepler_scale, "scale", A_SYM, 0);
        class_dspinit((t_class*const)class);
        class_register(CLASS_BOX, (t_class*const)class);
    }
}
