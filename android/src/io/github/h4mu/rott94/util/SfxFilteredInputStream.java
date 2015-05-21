/*
Copyright (C) 2014-2015 Tamas Hamor

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
package io.github.h4mu.rott94.util;

import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * @author hamu
 * see https://stackoverflow.com/questions/7924895/how-can-i-read-from-a-winzip-self-extracting-exe-zip-file-in-java
 */
public class SfxFilteredInputStream extends FilterInputStream {
	
	public SfxFilteredInputStream(InputStream in) {
		super(in);
	}

	public static final byte[] ZIP_LOCAL = { 0x50, 0x4b, 0x03, 0x04 };
	protected int ip;
	protected int op;

	public int read() throws IOException {
		while (ip < ZIP_LOCAL.length) {
			int c = super.read();
			if (c == ZIP_LOCAL[ip]) {
				ip++;
			} else
				ip = 0;
		}

		if (op < ZIP_LOCAL.length)
			return ZIP_LOCAL[op++];
		else
			return super.read();
	}

	public int read(byte[] b, int off, int len) throws IOException {
		if (op == ZIP_LOCAL.length)
			return super.read(b, off, len);
		int l = 0;
		while (l < Math.min(len, ZIP_LOCAL.length)) {
			b[l++] = (byte) read();
		}
		return l;
	}
}
