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
import android.provider.MediaStore;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class rott94Activity extends SDLActivity
{
    private static final String TAG = "Rott94";
    private MediaPlayer mediaPlayer;

    @Override
    protected String[] getArguments() {
        List<String> arguments = new ArrayList<>();
        File filesDir = getExternalFilesDir(null);
        if (filesDir == null) {
            filesDir = getFilesDir();
        }
        File cmdLine = new File(filesDir.getAbsolutePath() + File.separator + "arguments.txt");
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
            } catch (java.io.IOException ignored) {
            }
        }
        Uri data = getIntent().getData();
        if (data != null) {
            String[] projection = { MediaStore.Images.Media.DATA };
            Cursor cursor = getContentResolver().query(data, projection, null, null, null);
            if (cursor != null) {
                try {
                    int column_index = cursor.getColumnIndex(projection[0]);
                    if (column_index != -1 && cursor.moveToFirst()) {
                        String path = cursor.getString(column_index);
                        if (path != null && path.length() > 4) {
                            if (path.endsWith(".WAD")) {
                                arguments.add("FILE");
                                arguments.add(path);
                            }
                            else if (path.endsWith(".RTL")) {
                                arguments.add("FILERTL");
                                arguments.add(path);
                            }
                            else if (path.endsWith(".RTC")) {
                                arguments.add("FILERTC");
                                arguments.add(path);
                            }
                        }
                    }
                } finally {
                    cursor.close();
                }
            }
        }
        return arguments.toArray(new String[0]);
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
                "SDL2",
                // "SDL2_image",
                 "SDL2_mixer",
                // "SDL2_net",
                // "SDL2_ttf",
                "main"
        };
    }
}
