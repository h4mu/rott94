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
package io.github.h4mu.rott94;

import io.github.h4mu.rott94.util.SfxFilteredInputStream;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.widget.Toast;

/**
 * @author hamu
 * 
 */
public class ContentPrepareActivity extends Activity {
	private static final String SHAREWARE_URL = "https://github.com/h4mu/rott94/releases/download/v0.8-alpha/1rott13.zip";
	private static final int BUFFER_SIZE = 8192;

	static {
		System.loadLibrary("SDL2");
		System.loadLibrary("SDL2_mixer");
		System.loadLibrary("main");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		if (isGameContentInstalled()) {
			startActivity(new Intent(this, rott94Activity.class));
		} else {
			if (isShareware()) {
				new AlertDialog
				.Builder(this)
				.setCancelable(true)
				.setTitle(R.string.contentDownloadingTitle)
				.setMessage(R.string.contentDownloadingConfirmationMessage)
				.setPositiveButton(android.R.string.yes, new OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						downloadAndInstallGameContent();
					}
				}).setNegativeButton(android.R.string.no, new OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						ContentPrepareActivity.this.finish();
					}
				}).show();
			} else {
				Toast.makeText(this,
						getResources().getString(R.string.contentNotFoundMessage, getContentFolder().getAbsolutePath()),
						Toast.LENGTH_LONG).show();
				finish();
			}
		}
	}

	private void downloadAndInstallGameContent() {
		new AsyncTask<String, Void, Boolean>() {
			private ProgressDialog dialog = new ProgressDialog(ContentPrepareActivity.this);
			
			@Override
			protected void onPreExecute() {
				dialog.setTitle(R.string.contentDownloadingTitle);
				dialog.show();
			}

			@Override
			protected Boolean doInBackground(String... params) {
				try {
					ZipInputStream outerZip = new ZipInputStream(new BufferedInputStream(new URL(params[0]).openStream()));
					try {
						for (ZipEntry entry = outerZip.getNextEntry(); entry != null && !"ROTTSW13.SHR".equals(entry.getName()); entry = outerZip.getNextEntry()) {}
						ZipInputStream innerZip = new ZipInputStream(new SfxFilteredInputStream(outerZip));
						try {
							byte[] buffer = new byte[BUFFER_SIZE];
							for (ZipEntry entry = innerZip.getNextEntry(); entry != null; entry = innerZip.getNextEntry()) {
								FileOutputStream output = new FileOutputStream(getContentFolder().getAbsolutePath() + File.separator + entry.getName());
								try {
									for (int count = 0; (count = innerZip.read(buffer, 0, buffer.length)) >= 0; output.write(buffer, 0, count)) {}
								} finally {
									output.close();
								}
							}
						} finally {
							innerZip.close();
						}
					} finally {
						outerZip.close();
					}
				} catch (MalformedURLException e) {
					return false;
				} catch (IOException e) {
					return false;
				}
				return true;
			}
			
			@Override
			protected void onPostExecute(Boolean result) {
				if (dialog.isShowing()) {
					dialog.dismiss();
				}
				super.onPostExecute(result);
				if (result) {
					startActivity(new Intent(ContentPrepareActivity.this, rott94Activity.class));
				} else {
					finish();
				}
			}
		}.execute(SHAREWARE_URL);
	}

	private boolean isGameContentInstalled() {
		File contentDir = getContentFolder();
		return (contentDir.mkdirs() || contentDir.isDirectory())
				&& contentDir.list(new FilenameFilter() {

					@Override
					public boolean accept(File dir, String filename) {
						return "remote1.rts".equals(filename.toLowerCase())
								|| (isShareware()
										? "huntbgin.rtl".equals(filename.toLowerCase()) || "huntbgin.wad".equals(filename.toLowerCase())
										: "darkwar.rtl".equals(filename.toLowerCase()) || "darkwar.wad".equals(filename.toLowerCase()));
					}

				}).length >= 3;
	}

	private native boolean isShareware();

	private File getContentFolder() {
		File filesDir = getExternalFilesDir(null);
		if (filesDir == null) {
			filesDir = getFilesDir();
		}
		return filesDir;
	}
}
