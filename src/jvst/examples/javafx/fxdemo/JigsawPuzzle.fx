/* 
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * Copyright 2008 Sun Microsystems, Inc. All rights reserved. Use is subject to license terms. 
 * 
 * This file is available and licensed under the following license:
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * Neither the name of Sun Microsystems nor the names of its contributors 
 *     may be used to endorse or promote products derived from this software 
 *     without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* 
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * Copyright 2008 Sun Microsystems, Inc. All rights reserved. Use is subject to license terms. 
 * 
 * This file is available and licensed under the following license:
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * Neither the name of Sun Microsystems nor the names of its contributors 
 *     may be used to endorse or promote products derived from this software 
 *     without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * JigsawMain.fx
 *
 * Created on Oct 1, 2008, 6:12:45 PM
 */

package jvst.examples.javafx.fxdemo;

import javafx.stage.*;
import javafx.scene.*;
import javafx.scene.text.*;
import javafx.scene.paint.*;
import javafx.scene.shape.*;
import javafx.scene.image.*;
import javafx.scene.effect.*;
import javafx.scene.input.*;
import javafx.animation.*;
import java.lang.Math;
import java.lang.System;


public class JigsawPuzzle extends JavaInterop {
    var image = Image{
        url:"{__DIR__}puzzle_picture.jpg"};

    var maxCol:Integer = ((image.width as Integer) / 100) - 1;
    var maxRow:Integer = ((image.height as Integer) / 100) - 1;

    var pieces:JigsawPiece[];

    var scene : Scene;
    var timeline : Timeline;


      init {
        for (col in [0..maxCol]) {
            for (row in [0..maxRow]) {
                var x = col * 100;
                var y = row * 100;
                insert JigsawPiece{
                    image: image
                    correctX: x
                    correctY: y
                    topTab: row > 0
                    leftTab: col > 0
                    bottomTab: row < maxRow
                    rightTab: col < maxCol
                    onMousePressed: function (e:MouseEvent){
                        // stop any timelines so you can start dragging imediatly
                        if (timeline != null) timeline.stop();
                    }
                } into pieces;
            }
        }

        scene = Scene {
            width: 600
            height: 330
            fill: Color.rgb(100,100,100);
            content: Group {
                translateX: 50
                translateY: 50
                content: [
                    // Whole to place pieces in
                    Group {
                        cache:true
                        content: [
                            Rectangle {
                                effect: InnerShadow{
                                }
                                fill: Color.rgb(80,80,80);
                                stroke: Color.rgb(50,50,50);
                                width: 100 * (maxCol + 1)
                                height: 100 * (maxRow + 1)
                            },
                            // Lines
                            for (col in [0..(
                                maxCol - 1)]) {
                                Line{
                                    stroke: Color.rgb(70,70,70)
                                    startX: 100 + (100 * col)
                                    startY: 5
                                    endX: 100 + (100 * col)
                                    endY: 90 + (100 * maxRow)
                                }
                            }
                            for (row in [0..(
                                maxRow - 1)]) {
                                Line{
                                    stroke: Color.rgb(70,70,70)
                                    startX: 5
                                    startY: 100 + (100 * row)
                                    endX: 90 + (100 * maxCol)
                                    endY: 100 + (100 * row)
                                }
                            }
                        ]
                    },
                    // Buttons
                    TextButton {
                        translateX: bind (scene.width / 2) - 160
                        translateY: bind scene.height - 70
                        text: "Shuffle"
                        onMouseClicked: function (e:MouseEvent):Void{
                            if (timeline != null) timeline.stop();
                            timeline = Timeline {
                                repeatCount: 1
                                keyFrames: [
                                    KeyFrame {
                                        time: 0s;
                                        values:
                                        for (piece in pieces){
                                            [
                                                piece.inCorrectPlace => false,
                                                piece.translateX => piece.translateX,
                                                piece.translateY => piece.translateY
                                            ]
                                        }
                                    },
                                    KeyFrame {
                                        time: 5s;
                                        values:
                                        for (piece in pieces){
                                            [piece.translateX => (
                                                Math.random() * (scene.width - 150)) - 25 tween Interpolator.EASEIN,
                                                piece.translateY => (
                                                Math.random() * (scene.height - (150 + 35))) - 25 tween Interpolator.EASEIN]
                                        }
                                    }
                                ]
                            };
                            timeline.playFromStart();
                        }
                    },
                    TextButton {
                        translateX: bind (scene.width / 2) - 20
                        translateY: bind scene.height - 70
                        text: "Solve"
                        onMouseClicked: function (e:MouseEvent):Void{
                            if (timeline != null) timeline.stop();
                            timeline = Timeline {
                                repeatCount: 1
                                keyFrames: [
                                    KeyFrame {
                                        time: 0s;
                                        values:
                                        for (piece in pieces){
                                            [
                                                piece.translateX => piece.translateX,
                                                piece.translateY => piece.translateY
                                            ]
                                        }
                                    },
                                    KeyFrame {
                                        time: 5s;
                                        values:
                                        for (piece in pieces){
                                            [
                                                piece.inCorrectPlace => true,
                                                piece.translateX => piece.correctX,
                                                piece.translateY => piece.correctY
                                            ]
                                        }
                                    }
                                ]
                            };
                            timeline.playFromStart();
                        }
                    }
                    // Jigsaw Pieces
                    Group {
                        content: pieces
                    }
                ]
            }
          }


        timeline  = Timeline {
            repeatCount: 1
            keyFrames: [
                KeyFrame {
                    time: 3s;
                    values:
                    for (piece in pieces){
                        [piece.translateX => piece.translateX,
                            piece.translateY => piece.translateY]
                    }
                },
                KeyFrame {
                    time: 10s;
                    values:
                    for (piece in pieces){
                        [piece.translateX => (
                            Math.random() * (scene.width - 150)) - 25 tween Interpolator.EASEIN,
                            piece.translateY => (
                            Math.random() * (scene.height - (150 + 35))) - 25 tween Interpolator.EASEIN]
                    }
                }
            ]
        };

        timeline.playFromStart();
    }


    public override function getScene() : Object {
        return scene;
    }
    
}


