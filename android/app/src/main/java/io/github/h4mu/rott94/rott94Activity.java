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

import android.database.Cursor;
import android.media.MediaPlayer;
import android.net.Uri;
import android.provider.OpenableColumns;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;

public class rott94Activity extends SDLActivity
{
    private static final String TAG = "Rott94";
    private static final int BUFFER_SIZE = 8192;
    private MediaPlayer mediaPlayer;

    @Override
    protected String[] getArguments() {
        List<String> arguments = new ArrayList<>();
        File filesDir = getContentDirectory();
        File cmdLine = new File(filesDir, "arguments.txt");
        if (cmdLine.canRead()) {
            try {
                BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(cmdLine)));
                try {
                    String line = reader.readLine();
                    if (line != null) {
                        arguments.addAll(Arrays.asList(line.split(" ")));
                    }
                } finally {
                    reader.close();
                }
            } catch (IOException ignored) {
            }
        }

        Uri data = getIntent().getData();
        if (data != null) {
            addIntentFileArgument(arguments, data);
        }
        return arguments.toArray(new String[0]);
    }

    private void addIntentFileArgument(List<String> arguments, Uri data) {
        String displayName = getDisplayName(data);
        if (displayName == null) {
            displayName = data.getLastPathSegment();
        }
        if (displayName == null) {
            return;
        }

        String upperCaseName = displayName.toUpperCase(Locale.ROOT);
        String argumentName;
        if (upperCaseName.endsWith(".WAD")) {
            argumentName = "FILE";
        } else if (upperCaseName.endsWith(".RTL")) {
            argumentName = "FILERTL";
        } else if (upperCaseName.endsWith(".RTC")) {
            argumentName = "FILERTC";
        } else {
            return;
        }

        String path = resolveIntentFilePath(data, displayName);
        if (path == null) {
            return;
        }

        arguments.add(argumentName);
        arguments.add(path);
    }

    private String resolveIntentFilePath(Uri data, String displayName) {
        if ("file".equalsIgnoreCase(data.getScheme())) {
            return data.getPath();
        }

        try {
            return copyUriToImportCache(data, displayName).getAbsolutePath();
        } catch (IOException exception) {
            Log.w(TAG, "Could not import intent data", exception);
            return null;
        }
    }

    private File copyUriToImportCache(Uri data, String displayName) throws IOException {
        File importDir = new File(getContentDirectory(), "imports");
        if (!importDir.exists() && !importDir.mkdirs()) {
            throw new IOException("Could not create import directory");
        }

        File outFile = new File(importDir, sanitizeFilename(displayName));
        InputStream inputStream = getContentResolver().openInputStream(data);
        if (inputStream == null) {
            throw new IOException("Could not open URI stream");
        }

        try {
            FileOutputStream outputStream = new FileOutputStream(outFile);
            try {
                byte[] buffer = new byte[BUFFER_SIZE];
                for (int count; (count = inputStream.read(buffer)) != -1; ) {
                    outputStream.write(buffer, 0, count);
                }
            } finally {
                outputStream.close();
            }
        } finally {
            inputStream.close();
        }

        return outFile;
    }

    private String getDisplayName(Uri data) {
        Cursor cursor = getContentResolver().query(data, new String[] { OpenableColumns.DISPLAY_NAME }, null, null, null);
        if (cursor == null) {
            return null;
        }

        try {
            int nameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
            if (nameIndex != -1 && cursor.moveToFirst()) {
                return cursor.getString(nameIndex);
            }
        } finally {
            cursor.close();
        }
        return null;
    }

    private String sanitizeFilename(String filename) {
        return filename.replaceAll("[^A-Za-z0-9._-]", "_");
    }

    private File getContentDirectory() {
        File filesDir = getExternalFilesDir(null);
        if (filesDir == null) {
            filesDir = getFilesDir();
        }
        return filesDir;
    }

    private void playMusic(String midiFilePath, boolean shouldLoop) {
        try {
            if (mediaPlayer != null) {
                mediaPlayer.stop();
            }

            mediaPlayer = MediaPlayer.create(this, Uri.fromFile(new File(midiFilePath)));
            if (mediaPlayer != null) {
                mediaPlayer.setLooping(shouldLoop);
                mediaPlayer.start();
            }
        } catch(Exception exception) {
            Log.w(TAG, "PlayMusic: ", exception);
        }
    }

    @Override
    protected void onPause() {
        Log.v(TAG, "onPause()");
        if (mediaPlayer != null && mediaPlayer.isPlaying()) {
            mediaPlayer.pause();
        }
        super.onPause();
    }

    @Override
    protected void onResume() {
        Log.v(TAG, "onResume()");
        if (mediaPlayer != null && !mediaPlayer.isPlaying()) {
            mediaPlayer.start();
        }
        super.onResume();
    }

    @Override
    protected String[] getLibraries() {
        return new String[] {
                "hidapi",
                "SDL3",
                "SDL3_mixer",
                "main"
        };
    }
}
