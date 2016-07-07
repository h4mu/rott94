SUBTREE=android/jni/SDL
git subtree pull -P $SUBTREE --squash hg::http://hg.libsdl.org/SDL master
grep -lr '<<<<<<<' $SUBTREE | xargs git checkout --theirs
git add $SUBTREE/*
