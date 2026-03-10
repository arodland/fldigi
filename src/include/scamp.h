// ----------------------------------------------------------------------------
// scamp.h  --  SCAMP modem
//
// Copyright (C) 2012
//		Dave Freese, W1HKJ
//		Stefan Fendt, DL1SMF
//      Daniel Marks, KW4TI
//
// This file is part of fldigi.
//
// Fldigi is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Fldigi is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fldigi.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

#ifndef _SCAMP_H
#define _SCAMP_H

#include <iostream>

#include "complex.h"
#include "modem.h"
#include "globals.h"
#include "filters.h"
#include "fftfilt.h"
#include "digiscope.h"
#include "view_scamp.h"
#include "serial.h"

#include "scamp_protocol.h"

#define	SCAMP_SampleRate	8000

#define MAXPIPE			1024

#define dispwidth 100

#define SCAMP_TRANSBUFFER_MAX 1536
#define SCAMP_SAMPLE_COUNT_CHECK_RATE 500

class scamp : public modem {
public:

private:
    SCAMP_protocol scamp_protocol;
 
	double transbuffer[SCAMP_TRANSBUFFER_MAX];
    double transmit_phase;
 
    double symbol_freq_offset;
    double scamp_fsk_mode;
    
    fftfilt *mark_filt;
    fftfilt *space_filt;
    double mark_phase;
    double space_phase;
    int circbuffer_samples;
    double transmit_scale;
    int sample_count;
    int sample_count_check;
    double shift;
    double channel_bandwidth;
    double mode_bandwidth;
    double shift_freq;

    cmplx mixer(double &phase, double f, cmplx in) {
        cmplx z = cmplx(cos(phase), sin(phase)) * in;
        phase -= TWOPI * f / samplerate;
        if (phase < -TWOPI) phase += TWOPI;
        return z;
    }

	double *pipe;
	double *dsppipe;
	int pipeptr;

	double sigpwr;
	double noisepwr;

	void Clear_syncscope();
	void Update_syncscope();

	double IF_freq;

	view_scamp *scampviewer;

	void Metric();

	double scamp_now();
	int scamp_sleep (double);
    void send_frame(uint32_t frame);

public:
	scamp(trx_mode mode);
	~scamp();
	void init();
	void rx_init();
	void tx_init();
	void restart();
	void reset_filters();
	int rx_process(const double *buf, int len);
	int tx_process();
	void flush_stream();

	void clear_viewer() { scampviewer->clear(); }
	void clear_ch(int n) { scampviewer->clearch(n); }
//	int  viewer_get_freq(int n) { return scampviewer->get_freq(n); }

	void searchDown();
	void searchUp();
};

int scampparity(unsigned int, int);

#endif
