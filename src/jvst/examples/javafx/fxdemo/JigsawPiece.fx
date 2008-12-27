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
 * JigsawPiece.fx
 *
 * Created on Oct 1, 2008, 6:54:10 PM
 */

package jvst.examples.javafx.fxdemo;

import javafx.scene.*;
import javafx.scene.shape.*;
import javafx.scene.image.*;
import javafx.scene.paint.*;
import javafx.scene.input.*;
import javafx.scene.effect.*;
import java.lang.System;

/**
 * @author jp202575
 */
public class JigsawPiece extends CustomNode {

    public-init var image:Image;
    public-init var correctX:Number;
    public-init var correctY:Number;
    public-init var topTab:Boolean = true;
    public-init var leftTab:Boolean = true;
    public-init var bottomTab:Boolean = true;
    public-init var rightTab:Boolean = true;

    public var inCorrectPlace:Boolean = false on replace {
        if (inCorrectPlace) {
            group.effect = null;
            jigsawPiece.stroke = Color.rgb(0,0,0,0.5);
            toBack();
        } else {
            group.effect = DropShadow {};
            jigsawPiece.stroke = Color.BLACK;
            toFront();
        }
    };

    var group:Group;
    var startDragX:Number;
    var startDragY:Number;
    var jigsawPiece:Shape;

    init {
        cache = true;
        translateX = correctX;
        translateY = correctY;
        jigsawPiece = createPiece(0,0,topTab,leftTab,bottomTab,rightTab);
        var jigsawPieceClip = createPiece(correctX,correctY,topTab,leftTab,bottomTab,rightTab);
        
        // fix for issue of complex cilp not handling mouse events correctly
        jigsawPiece.fill = Color.rgb(0,0,0,0);

        group = Group {
            cache: true
            blocksMouse: true
            content: [
                ImageView {
                    translateX: -correctX
                    translateY: -correctY
                    image: image;
                    clip: jigsawPieceClip
                },
                jigsawPiece
            ]
            effect: DropShadow {
            };
            onMousePressed: function(e:MouseEvent):Void {
                if (not inCorrectPlace){
                    toFront();
                    startDragX = translateX;
                    startDragY = translateY;
                }
            }
            onMouseReleased: function(e:MouseEvent):Void {
                if (not inCorrectPlace){
                    // check if
                    if (translateX < (correctX + 10) and translateX > (correctX - 10) and
                    translateY < (correctY + 10) and translateY > (correctY - 10)){
                        translateX = correctX;
                        translateY = correctY;
                        inCorrectPlace = true;
                    }
                }
            }
            onMouseDragged: function(e:MouseEvent):Void {
                if (not inCorrectPlace){
                    translateX = startDragX + e.dragX;
                    translateY = startDragY + e.dragY;
                }
            }
        };
    }

    protected override function create():Node {
        return group;
    }

    function createPiece(x:Number, y:Number, topTab:Boolean, leftTab:Boolean, bottomTab:Boolean, rightTab:Boolean): Shape {
        ShapeSubtract {
            translateX: x + 50
            translateY: y + 50
            fill: null
            stroke: Color.BLACK
            a: [
                // main rectangle
                Rectangle {
                    x: -50
                    y: -50
                    width: 100
                    height: 100
                }
                // right tab
                if (rightTab) {
                    ShapeSubtract {
                        a: [
                            Ellipse {
                                centerX: 69.5
                                centerY: 0
                                radiusX: 10
                                radiusY: 17.5
                            },
                            Rectangle {
                                x: 50
                                y: -12.5
                                width: 11.5
                                height: 25
                            }
                        ]
                        b: [
                            Circle {
                                centerX: 56.25
                                centerY: -14
                                radius: 6.25
                            },
                            Circle {
                                centerX: 56.25
                                centerY: 14
                                radius: 6.25
                            }
                        ]

                    }
                } else { null
                }
                // bottom tab
                if (bottomTab) {
                    ShapeSubtract {
                        a: [
                            Ellipse {
                                centerX: 0
                                centerY: 69.5
                                radiusX: 17.5
                                radiusY: 10
                            },
                            Rectangle {
                                x: -12.5
                                y: 50
                                width: 25
                                height: 11
                            }
                        ]
                        b: [
                            Circle {
                                centerX: -14
                                centerY: 56.25
                                radius: 6.25
                            },
                            Circle {
                                centerX: 14
                                centerY: 56.25
                                radius: 6.25
                            }
                        ]

                    }
                } else { null
                }
            ]
            b: [
                // left tab
                if (leftTab) {
                    ShapeSubtract {
                        a: [
                            Ellipse {
                                centerX: -31
                                centerY: 0
                                radiusX: 10
                                radiusY: 17.5
                            },
                            Rectangle {
                                x: -50
                                y: -12.5
                                width: 11
                                height: 25
                            }
                        ]
                        b: [
                            Circle {
                                centerX: -43.75
                                centerY: -14
                                radius: 6.25
                            },
                            Circle {
                                centerX: -43.75
                                centerY: 14
                                radius: 6.25
                            }
                        ]
                    }
                } else { null
                }
                // top tab
                if (topTab) {
                    ShapeSubtract {
                        a: [
                            Ellipse {
                                centerX: 0
                                centerY: -31
                                radiusX: 17.5
                                radiusY: 10
                            },
                            Rectangle {
                                x: -12.5
                                y: -50
                                width: 25
                                height: 12.5
                            }
                        ]
                        b: [
                            Circle {
                                centerX: -14
                                centerY: -43.75
                                radius: 6.25
                            },
                            Circle {
                                centerX: 14
                                centerY: -43.75
                                radius: 6.25
                            }
                        ]

                    }
                } else { null
                }
            ]
            }
    }
}
