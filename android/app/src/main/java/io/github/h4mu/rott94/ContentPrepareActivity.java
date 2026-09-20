/*
Copyright (C) 2014-2019 Tamas Hamor

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

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.net.URL;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import io.github.h4mu.rott94.util.SfxFilteredInputStream;

public class ContentPrepareActivity extends Activity {
    private static final String SHAREWARE_URL = "https://github.com/h4mu/rott94/releases/download/v0.8-alpha/1rott13.zip";
    private static final int BUFFER_SIZE = 8192;

    private final ExecutorService backgroundExecutor = Executors.newSingleThreadExecutor();
    private ProgressDialog downloadDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (isGameContentInstalled()) {
            launchGameActivity();
        } else if (BuildConfig.SHAREWARE) {
            showSharewareDownloadPrompt();
        } else {
            Toast.makeText(
                    this,
                    getResources().getString(R.string.contentNotFoundMessage, getContentFolder().getAbsolutePath()),
                    Toast.LENGTH_LONG
            ).show();
            finish();
        }
    }

    @Override
    protected void onDestroy() {
        backgroundExecutor.shutdownNow();
        super.onDestroy();
    }

    private void showSharewareDownloadPrompt() {
        new AlertDialog.Builder(this)
                .setCancelable(true)
                .setTitle(R.string.contentDownloadingTitle)
                .setMessage(R.string.contentDownloadingConfirmationMessage)
                .setPositiveButton(android.R.string.yes, (dialog, which) -> downloadAndInstallGameContent())
                .setNegativeButton(android.R.string.no, (dialog, which) -> finish())
                .show();
    }

    private void downloadAndInstallGameContent() {
        downloadDialog = new ProgressDialog(this);
        downloadDialog.setTitle(R.string.contentDownloadingTitle);
        downloadDialog.setCancelable(false);
        downloadDialog.show();

        backgroundExecutor.execute(() -> {
            final boolean result = installSharewareContent(SHAREWARE_URL);
            runOnUiThread(() -> onDownloadFinished(result));
        });
    }

    private boolean installSharewareContent(String url) {
        try {
            ZipInputStream outerZip = new ZipInputStream(new BufferedInputStream(new URL(url).openStream()));
            try {
                for (ZipEntry entry = outerZip.getNextEntry();
                     entry != null && !"ROTTSW13.SHR".equals(entry.getName());
                     entry = outerZip.getNextEntry()) {
                }

                ZipInputStream innerZip = new ZipInputStream(new SfxFilteredInputStream(outerZip));
                try {
                    byte[] buffer = new byte[BUFFER_SIZE];
                    for (ZipEntry entry = innerZip.getNextEntry(); entry != null; entry = innerZip.getNextEntry()) {
                        File outputFile = new File(getContentFolder(), entry.getName());
                        File parent = outputFile.getParentFile();
                        if (parent != null && !parent.exists() && !parent.mkdirs()) {
                            return false;
                        }

                        FileOutputStream output = new FileOutputStream(outputFile);
                        try {
                            for (int count; (count = innerZip.read(buffer, 0, buffer.length)) >= 0; ) {
                                output.write(buffer, 0, count);
                            }
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
            return true;
        } catch (IOException exception) {
            return false;
        }
    }

    private void onDownloadFinished(boolean result) {
        if (downloadDialog != null && downloadDialog.isShowing()) {
            downloadDialog.dismiss();
        }

        if (result) {
            launchGameActivity();
        } else {
            finish();
        }
    }

    private void launchGameActivity() {
        startActivity(new Intent(getIntent().getAction(), getIntent().getData(), this, rott94Activity.class));
    }

    private boolean isGameContentInstalled() {
        File contentDir = getContentFolder();
        String[] entries = (contentDir.mkdirs() || contentDir.isDirectory())
                ? contentDir.list(new FilenameFilter() {
                    @Override
                    public boolean accept(File dir, String filename) {
                        String upperCase = filename.toUpperCase();
                        if (!filename.equals(upperCase)) {
                            new File(dir, filename).renameTo(new File(dir, upperCase));
                        }
                        return "REMOTE1.RTS".equals(upperCase)
                                || (BuildConfig.SHAREWARE
                                ? "HUNTBGIN.RTL".equals(upperCase) || "HUNTBGIN.WAD".equals(upperCase)
                                : "DARKWAR.RTL".equals(upperCase) || "DARKWAR.WAD".equals(upperCase));
                    }
                })
                : null;
        return entries != null && entries.length >= 3;
    }

    private File getContentFolder() {
        File filesDir = getExternalFilesDir(null);
        if (filesDir == null) {
            filesDir = getFilesDir();
        }
        return filesDir;
    }
}
